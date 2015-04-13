saveMatrixBin = function(fileName, m) {
    binFile = file(fileName, 'wb')
    on.exit(close(binFile))
    writeBin(dim(m), binFile)
    writeBin(c(m), binFile, size = 4) 
    flush(binFile)
}

saveMatrixRData = function(fileName, m1) {
    save(m, file = fileName)
}

saveMatrixCsv = function(fileName, m) {
    m = format(m, nsmall = 10) 
    write.table(m, fileName, row.names = FALSE, col.names = FALSE, sep = ";", quote = FALSE)
}

saveMatrix = function(fileName, m) {
    #saveMatrixBin(paste(fileName, "bin", sep = "."), m)
    #saveMatrixRData(paste(fileName, "RData", sep = "."), m)
    saveMatrixCsv(paste(fileName, "csv", sep = "."), m)
}

getAnnotation = function(probeInfoFile = "probeInfo.RData", probesOfInterest = NULL,
                         removeNonUnique = FALSE, minProbesPerGene = 5) {
    
    load(probeInfoFile)
    annotation = data.frame(as.character(probeInfo[,"ID"]),
                            as.integer(probeInfo[,"MAPINFO"]),
                            as.character(probeInfo[,"SYMBOL"]),
                            stringsAsFactors = FALSE)
    names(annotation) = c("ID", "COORD", "GENE")
    rm(probeInfo)
    
    if (!is.null(probesOfInterest)) {
        annotation = annotation[annotation$ID %in% probesOfInterest,]
    }
    
    # sort annotation by gene and for each gene by CG-pair coordinate
    annotation = annotation[order(annotation$GENE, annotation$COORD),]

    # remove probes with non-unique mappings    
    if (removeNonUnique) {
        deltaCoordinate = diff(sort(annotation$COORD))
        badMapping = which(deltaCoordinate == 0)
        annotation = annotation[-order(annotation$COORD)[c(badMapping, badMapping + 1)],]
    }
    
    # remove genes with insufficient numer of probes
    probesPerGene = sort(table(annotation$GENE), decreasing = TRUE)
    genesOfInterest = names(which(minProbesPerGene <= probesPerGene))
    annotation = annotation[annotation$GENE %in% genesOfInterest,]
    
    missedProbes = probesOfInterest[!(probesOfInterest %in% annotation$ID)]
    #if (0 < length(missedProbes)) {
    #    warning(paste0(c("The following probes are missed in the annotation:",
    #                     missedProbes), collapse = "\n"))
    #}
    
    return(annotation)
}


calcMean = function(dataB, probeGroups, rowsMatch) {    
    # calculate mean 
    betaMean = sapply(probeGroups, function(probes) {
        probes = unlist(probes)
        tempData = dataB[rowsMatch[probes],]
        if (is.null(dim(tempData))) {
          tempResu = tempData
        } else {
          tempResu = apply(tempData, 2, mean)
        }
        return(tempResu)
    })
    return(betaMean)
}

calcVar = function(dataB, probeGroups, rowsMatch) {
    # calculate variance
    betaVar = sapply(probeGroups, function(probes) {
      probes = unlist(probes)
      tempData = dataB[rowsMatch[probes],]
      if (is.null(dim(tempData))) {
        tempResu = tempData
      } else {
        tempResu = apply(tempData, 2, var)
      }
        return(tempResu)
    })
    return(betaVar)
}

calcDev = function(dataB, probeGroups, annotation, rowsMatch) {
    # calculate difference from line, not standart deviation
    betaDeviation = sapply(probeGroups, function(probe) {
        m = length(probe)
        x = annotation[probe, "COORD"]
        x1 = x[1]
        x2 = x[m]
        x = x - x1
        coeff = 1. / (x2 - x1)
        return(apply(dataB[rowsMatch[probe],], 2, function(p) {
            y1 = p[1]
            y2 = p[m]
            return(sum((y2 - y1) * coeff * x + y1 - p))
        }))
    })
    return(betaDeviation)
}

calcMeanDeriv = function(dataB, probeGroups, rowsMatch) {
    # calculate mean absolute derivative
    betaMeanDeriv = sapply(probeGroups, function(probe) {
        return(apply(dataB[rowsMatch[probe],], 2, function(p) {
            return(mean(abs(diff(p))))
        }))
    })
    return(betaMeanDeriv)
}


calcAsymmetry = function(dataB, probeGroups, rowsMatch) {
    # calculate mean absolute derivative
    betaMeanDeriv = sapply(probeGroups, function(probe){
        return(apply(dataB[rowsMatch[probe],], 2, function(p) {
            derivative = diff(p)
            return( sum(derivative[0 < derivative]) / sum(abs(derivative)) )
        }))
    })
    return(betaMeanDeriv)
}

calcAllMeasures = function(dataB, annotation, features = c("mean", "var"), genesOfInterest = NULL) {

    annotation = annotation[annotation$ID %in% rownames(dataB),]

  	if (!is.null(genesOfInterest)) {
  		if (!all(genesOfInterest %in% annotation$GENE)) {
  			warning("Some genes of interest are missed in annotation.")
  		}
  	} else {
  		genesOfInterest = names(sort(table(annotation$GENE), decreasing = TRUE))
  	}
	
    annotation = annotation[annotation$GENE %in% genesOfInterest,]
    
    # group CpG probes corresponding to the same gene
    probeGroups = lapply(genesOfInterest, function(gn){ which(annotation$GENE == gn) })
    names(probeGroups) = genesOfInterest
    
    rowsMatch = match(annotation$ID, rownames(dataB))
    
    if ("mean" %in% features){
        betaMean = calcMean(dataB, probeGroups, rowsMatch)
        saveMatrix("mean", betaMean)
        rm(betaMean)
    }
    
    if ("var" %in% features){
        betaVar = calcVar(dataB, probeGroups, rowsMatch)
        saveMatrix("var", betaVar)
        rm(betaVar)
    }
    
    if ("dev" %in% features){
        betaDeviation = calcDev(dataB, probeGroups, annotation, rowsMatch)
        saveMatrix("dev", betaDeviation)
        rm(betaDeviation)
    }
    
    if ("deriv" %in% features){
        betaMeanDeriv = calcMeanDeriv(dataB, probeGroups, rowsMatch)
        saveMatrix("deriv", betaMeanDeriv)
        rm(betaMeanDeriv)
    }
    
    if ("asymm" %in% features){
        betaAsymmetry = calcAsymmetry(dataB, probeGroups, rowsMatch)
        saveMatrix("asymm", betaAsymmetry)
        rm(betaAsymmetry)
    }
    
    return(genesOfInterest)
}


calcFeatures = function(fileName, 
                        probeInfoFile = "probeInfo.RData",
                        probesNamesFile = "probes_of_interest.csv",
						            geneNamesFile = "genes_of_interest.csv",
                        features = c("mean", "var")) {
    
    load(probesNamesFile)
    
    genesOfInterest = NULL
    if (!is.null(geneNamesFile)) {
      load(geneNamesFile)
    }
    load(fileName)
    ann = getAnnotation(probeInfoFile, probesOfInterest)
    genesOfInterest = calcAllMeasures(dataB, ann, features, genesOfInterest)
    saveMatrixCsv("genes_of_interest.csv", genesOfInterest)
}

preprocessData = function(dataFileNameIn, dataFileNameOut) {
    load(dataFileNameIn)
    # calculate coverage for each probe as the proportion of samples with NA values
    coverage = apply(dataB, 1, function(DT){ length(which(!is.na(DT))) }) / dim(dataB)[2]
    # remove probes with less than 95% coverage
    dataB = dataB[coverage >= 0.95,]
    # percentage of NA values
    percentNA = 100 * length(which(is.na(dataB))) / length(dataB)
    print(paste(signif(percentNA, digits = 3), "% NA", sep = "")) 
    if (any(is.na(dataB))) {
        # if necessary, replace NA values by imputation
        if (percentNA <= 1.5) {
            pckg = require(impute)
            if (!pckg) {
                cat("Installing 'impute' from Bioconductor\n")
                source("http://www.bioconductor.org/biocLite.R")
                biocLite("impute")
                library(impute)
            }
            #impute to replace NA values
            dataB = impute.knn(dataB, k = 5)$data
        }
    }
    save(dataB, file = dataFileNameOut)
}



system.time(calcFeatures("./Processed/BdataTCGA__BRCA.rd",
                         probeInfoFile = "./Processed/probeInfo.rd",
                         probesNamesFile = "./Processed/probes_of_interest.Rd",
                         geneNamesFile = "./Processed/genes_of_interest.Rd"))

