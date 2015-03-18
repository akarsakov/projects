data_path <- "D:/Dev/bio/data"
setwd(data_path)

files <- dir(".", pattern="csv")
#files <- c("geneMeanMatsTCGABRCA5.rd")
file <- files[1]

#for (file in files) {
  setwd(data_path)

  cancerType <- sub(".csv", "", file)
  
  print(paste("Process:", cancerType))
  
  data <- read.csv(file, header=F, sep=",", dec=".")
  phen <- data["V1"] == "cancer"
  
  numCancer <- sum(phen)
  numHealthy <- sum(!phen)
  minExamples <- min(numCancer, numHealthy)
  
  indexes <- seq_along(phen)
  cancerIndexes <- indexes[phen]
  healthyIndexes <- indexes[!phen]
  
  
  if (minExamples < 50) {
    numChunks <- 2
  } else {
    numChunks <- 4
  }
  
  # shuffle indexes
  cancerIndexes <- sample(cancerIndexes)
  healthyIndexes <- sample(healthyIndexes)
  
  cancerChunks <- split(1:numCancer, cut(1:numCancer, numChunks))
  healthyChunks <- split(1:numHealthy, cut(1:numHealthy, numChunks))
  
  dir.create(file.path(data_path, cancerType))
  setwd(file.path(data_path, cancerType))
  
  for (i in 1:numChunks) {
    testCancer <- cancerIndexes[cancerChunks[[i]]]
    trainCancer <- cancerIndexes[!(cancerIndexes %in% testCancer)]
    testHealthy <- healthyIndexes[healthyChunks[[i]]]
    trainHealthy <- healthyIndexes[!(healthyIndexes %in% testHealthy)]
    
    write.table(data[trainCancer, ], file=paste(cancerType, "_train_cancer_", i, ".csv", sep=""),
                row.names=F, col.names=F, quote=F, sep=",")
    write.table(data[trainHealthy, ], file=paste(cancerType, "_train_healthy_", i, ".csv", sep=""),
                row.names=F, col.names=F, quote=F, sep=",")
    
    testIndexes <- c(testCancer, testHealthy)
    write.table(data[testIndexes, ], file=paste(cancerType, "_test_", i, ".csv", sep=""),
                row.names=F, col.names=F, quote=F, sep=",")
    
    testLabels <- c(rep("cancer", length(testCancer)), rep("healthy", length(testHealthy)))
    write.table(testLabels, file=paste(cancerType, "_test_labels", i, ".csv", sep=""),
                row.names=F, col.names=F, quote=F, sep=",")
  }
#}