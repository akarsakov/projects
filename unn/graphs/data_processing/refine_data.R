trim <- function( x ) {
    gsub("(^[[:space:]]+|[[:space:]]+$)", "", x)
}

data <- read.delim("mean.csv", header=F, sep=";")
names <- read.csv("genes_of_interest.csv", header=F)

load("./Processed/phenDataTCGA__BRCA.rd")

patients <- names(phenotypes.lv$batch)
clinic_data <- read.delim("./Processed/clinic_data.txt", header=T, sep="\t", as.is=T, skip=1)
days_to_death <- c()

for (i in 1:length(patients)) {
    name <- substr(patients[i], 1, 12)
    days_to_death <- c(days_to_death, clinic_data[clinic_data["bcr_patient_barcode"] == name, "days_to_death"])
}

good_genes <- c()
for (i in 1:ncol(data)) {
    good_genes <- c(good_genes, sum(is.na(data[,i])) == 0)
}
    
sum(good_genes)

colnames(data) <- trim(names[,1])
data <- data[,good_genes]
data <- cbind(patients,days_to_death, data)

write.csv(data, "mean_refined.csv", row.names=F, quote=F)
