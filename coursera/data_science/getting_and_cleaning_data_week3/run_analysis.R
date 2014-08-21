# read activity labels
activity_labels <- read.table("activity_labels.txt", sep=" ", col.names=c("No", "Activity.Name"), as.is=T)

# read feature names
features <- read.table("features.txt", sep=" ", col.names=c("No", "Feature.Name"), as.is=T)

# read train data
# first, read subjects train
subjects_train <- read.table("train/subject_train.txt", col.names=c("Subject.ID"))
# read train X and y
train_data_x <- read.table("train/X_train.txt", sep=" ", stringsAsFactors=F)
