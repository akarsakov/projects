rm(list=ls())

#Setting up work directory
base = "D:/Dev/bio/processing/data/"
work_dir <- base #paste(base,"Raw",sep="")
processed_dir = paste(base,"Processed",sep="")
setwd(work_dir)

#bulk data matrices to process
zippedNames<-dir();

    zName = zippedNames[1]

	##decompress data
	#zName2<-strsplit(zName,".",fixed=T)[[1]][1];
	#untar(zName,compressed="gzip",exdir=zName2);#decompress files
	setwd(zName)
	
    ##Enter extracted folder
	##extract batch info
	setwd("METADATA")
	setwd(dir())
  
	#Name of file containing metadata
	tempName<-dir()[grepl("sdrf",dir(),fixed=T)];
  
	#name of tumour type
	tName<-strsplit(strsplit(tempName,"_",fixed=T)[[1]][2],".",fixed=T)[[1]][1];
  
	#meta data
	metaData<-read.table(tempName,header=T,sep="\t",as.is=T,quote="",na.strings="null");
  
	#batch info
	batchInfoTemp<-metaData[,c("Comment..TCGA.Barcode.","Comment..TCGA.Archive.Name..2")];
	colnames(batchInfoTemp)<-c("barcode","batch")
	batchInfo<-sapply(strsplit(sapply(strsplit(batchInfoTemp[,"batch"],"Level_3",fixed=T),function(xx){return(xx[2])}),".",fixed=T),function(yy){return(yy[2])});#retain batch numbers
	names(batchInfo)<-batchInfoTemp[,"barcode"]
	
	##read in data files
	setwd("../../DNA_Methylation")
	setwd(dir())
	setwd("Level_3")
  
	#names of data files
	fileNames<-dir();
  
	#probe names in first data file, to match to
	refProbeOrd<-read.table(fileNames[1],header=T,sep="\t",as.is=T,quote="",na.strings="null",skip=1)[,"Composite.Element.REF"];
	
  
  dataB<-as.matrix(data.frame(unlist(sapply(fileNames,function(FN){
    #load data, with time taken to load
		tempT<-system.time(tempData<-read.table(FN,header=T,sep="\t",as.is=T,quote="",na.strings="null",skip=1))[[3]];
		print(paste(FN,signif(tempT,digits=2),"s"))
    
		#order samples from each file the same
		tempOrd<-match(refProbeOrd,tempData[,"Composite.Element.REF"]);
		tempData<-as.numeric(tempData[tempOrd,"Beta_value"]);
		names(tempData)<-refProbeOrd;
		tempData<-list(temp=tempData);
		tempName<-as.vector(as.matrix(read.table(FN,header=F,nrow=1,sep="\t",as.is=T)))
		tempName<-unique(tempName[grepl("TCGA",tempName,fixed=T)])
		names(tempData)<-tempName;
		return(tempData)
	},simplify=F,USE.NAMES=F),recursive=F)))
	colnames(dataB)<-gsub(".","-",colnames(dataB),fixed=T)

	##assess phenotype info
	phenCodes<-as.numeric(sapply(strsplit(colnames(dataB),"-",fixed=T),function(xx){return(substr(xx[4],1,2))}));#phenotype info
	
	#samples to remove, as neither tumour or healthy
    extraSamples<-phenCodes>=20;
  
	#remove extra samples
	dataB<-dataB[,!extraSamples];
	phen<-rep(0,ncol(dataB));
  
	#tumour samples
	phen[phenCodes<10]<-1;
	phen<-as.logical(phen);
  
	#empty vector for phenotype list
	disease_status<-rep("healthy",length(phen));
  
	#add phenotype labels in appropriate places
	disease_status[phen]<-"cancer";
  
	#corresponding batch info
	batch<-batchInfo[colnames(dataB)];
  
	#array used for the processing
	plate<-sapply(strsplit(colnames(dataB),"-",fixed=T),function(xx){return(xx[6])});
	phenotypes.lv<-list(batch=batch,plate=plate,disease_status=disease_status)

	###Check coverage, and remove probes/replace missing values as appropriate
  ###
	#calculate coverage for each probe as the proportion of samples with NA values
	coverage<-apply(dataB,1,function(DT){length(which(!is.na(DT)))})/dim(dataB)[2];
	
	#remove probes with less than 95% coverage
  dataB<-dataB[coverage>=0.95,];
  
	#percentage of NA values
	percentNA<-100*length(which(is.na(dataB)))/length(dataB);
	print(paste(tName,": ",signif(percentNA,digits=3),"% NA",sep="")) 
	
	#if necessary, replace NA values by imputation
  if(any(is.na(dataB))){ 
		if(percentNA<=1.5){
		pckg = try(require(impute));
			if(!pckg){
				cat("Installing 'impute' from Bioconductor\n");
				source("http://www.bioconductor.org/biocLite.R");
				biocLite("impute");
				require(impute);
			};
		  #impute to replace NA values
			dataB<-impute.knn(dataB,k=5)$data;
		 }
	}
  
	#setwd("../../../../..")
	setwd(processed_dir)
	save(list="dataB",file=paste("BdataTCGA__",tName,".rd",sep=""))
	save(list=c("phenotypes.lv","phen"),file=paste("phenDataTCGA__",tName,".rd",sep=""))
	rm(list="dataB")
	unlink(zName2,recursive=T,force=T)

setwd(processed_dir)
save(list="tumourTypes",file="tumourTypesMeth.rd")
