library(e1071)
data_path <- "D:/Dev/bio/data/BLCA/test_0/"

setwd(data_path)

labels <- read.csv("test_labels.csv", header=F)
labels <- c(labels == "cancer")

indexes <- seq_along(labels)
cancerIndexes <- indexes[labels]
healthyIndexes <- indexes[!labels]
results <- data.frame(Dir=character(),
                 classificationRate=double(), 
                 healthyRate=double(), 
                 cancerRate=double())

test_dirs <- dir()[file.info(dir())$isdir]

for (cur_dir in test_dirs) {
    data <- read.csv(paste(data_path, cur_dir,"/metrics.csv", sep=""))
    data$max_degree <- NULL 
    
    trainCancer <- sample(cancerIndexes, length(cancerIndexes)/2)
    trainHealthy <- sample(healthyIndexes, length(healthyIndexes)/2)
    trainData <- data[(seq_along(labels) %in% trainCancer) | (seq_along(labels) %in% trainHealthy),]
    testData <- data[!((seq_along(labels) %in% trainCancer) | (seq_along(labels) %in% trainHealthy)),]
    
    trainLabels <- labels[(seq_along(labels) %in% trainCancer) | (seq_along(labels) %in% trainHealthy)]
    testLabels <- labels[!((seq_along(labels) %in% trainCancer) | (seq_along(labels) %in% trainHealthy))]
    
    trainData$label <- trainLabels
    model <- svm(label~., data = trainData, type="C-classification")
    result <- predict(model, testData)
    t <- table(result, testLabels)
    
    classRate <- (t[1] + t[4]) / sum(t)
    healthyRate <- t[1] / (t[1] + t[2])
    cancerRate <- t[4] / (t[3] + t[4])
    print(cur_dir)
    results[nrow(results)+1, ] <- list(paste(cur_dir, "!"), classRate, healthyRate, cancerRate)
}

results$Dir <- test_dirs
results
