## 0. reading files
# read activity labels
activity_labels <- read.table("activity_labels.txt", sep=" ", col.names=c("No", "Activity.Name"), as.is=T)

# read feature names
features <- read.table("features.txt", sep=" ", col.names=c("No", "Feature.Name"), as.is=T)

# read train data
subjects_train <- read.table("train/subject_train.txt", col.names=c("Subject"))
train_data_x <- read.table("train/X_train.txt", header=F, col.names=features$Feature.Name, strip.white=TRUE)
train_data_y <- read.table("train/y_train.txt", header=F, colClasses=c("factor"), col.names=c("Activity"))

# read test data
subjects_test <- read.table("test/subject_test.txt", col.names=c("Subject"))
test_data_x <- read.table("test/X_test.txt", header=F, col.names=features$Feature.Name, strip.white=TRUE)
test_data_y <- read.table("test/y_test.txt", header=F, colClasses=c("factor"), col.names=c("Activity"))

## 1. merge the training and the test sets to create one data set.
full_data_set <- cbind(train_data_x, subjects_train, train_data_y)
full_data_set <- rbind(full_data_set, cbind(test_data_x, subjects_test, test_data_y))

## 2. extract only mean and stddev variables
reduced_data_set <- full_data_set[, c(grep("(mean|std)", names(full_data_set)), ncol(full_data_set)-1, ncol(full_data_set))]

## 3. make descriptive activity names
levels(reduced_data_set$Activity) <- activity_labels$Activity.Name

## 4. make descriptive variable names
# remove dots
names(reduced_data_set) <- gsub("\\.", "", names(reduced_data_set))
# mean -> uppercase
names(reduced_data_set) <- gsub("mean", "Mean", names(reduced_data_set))
# std -> uppercase
names(reduced_data_set) <- gsub("std", "Std", names(reduced_data_set))
# prefix 't' -> Time
names(reduced_data_set) <- gsub("^t", "Time", names(reduced_data_set))
# prefix 'f' -> FFT
names(reduced_data_set) <- gsub("^f", "FFT", names(reduced_data_set))

## 5. create data set with the average of each variable for each activity and each subject
result <- aggregate(. ~ Subject + Activity, data=reduced_data_set, FUN="mean")

## write result to file
write.table(result, file="tidy_data.txt", row.names=F)
