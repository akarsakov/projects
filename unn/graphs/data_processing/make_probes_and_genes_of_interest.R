#load file probeInfo
path = "./Processed/probeInfo.rd"
load(path)

#remove everything without annotateg Gene 
probeInfo<-probeInfo[!is.na(probeInfo[,4]),]

# extract genes and probes
probesOfInterest = unique(probeInfo[,1])
genesOfInterest = unique(probeInfo[,5])

save(list="probesOfInterest", file="./Processed/probes_of_interest.Rd")
save(list="genesOfInterest", file="./Processed/genes_of_interest.Rd")