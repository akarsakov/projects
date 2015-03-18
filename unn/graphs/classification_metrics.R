library(e1071);
library(randomForest)

setwd('D:/Dev/bio/scripts/m2')

cancerData = read.table("metrics_cancer.csv", header=TRUE, sep=',')
cancerData$X <- NULL
cancerData$min_weight <- NULL
cancerData$min_degree <- NULL
cancerData["class"] <- "cancer"

healthyData = read.table("metrics_healthy.csv", header=TRUE, sep=',')
healthyData$X <- NULL
healthyData$min_weight <- NULL
healthyData$min_degree <- NULL
healthyData["class"] <- "healthy"

num_tests = 100
error = vector(mode="numeric", length=num_tests)
error_rf = vector(mode="numeric", length=num_tests)
for (i in 1:num_tests)
{
index <- sample(1:nrow(cancerData), 30)
trainCancer = cancerData[index,]
testCancer = cancerData[-index,]

index <- sample(1:nrow(healthyData), 30)
trainHealth = healthyData[index,]
testHealth = healthyData[-index,]

train = rbind(trainCancer, trainHealth)
test = rbind(testCancer, testHealth)
rownames(train) <- NULL
rownames(test) <- NULL

#print("SVM")
svm_model <- svm( train$class ~ ., train, kernel = "radial", type="C-classification" )
x_test <- subset(test, select = -class);
predy <- predict(svm_model, x_test);
table <- table(test$class, predy );

#print(paste("Error 1 (wrong healthy predict):", table[1,2]/sum(table)))
#print(paste("Error 2 (wrong cancer predict):", table[2,1]/sum(table)))
#print(paste("Error: ", sum(diag(table))/sum(table)))
error[i] = sum(diag(table))/sum(table)

#print("RF")
rf <- randomForest(as.factor(class) ~ ., train, n.tree=10000)
predict_rf <- predict(rf, test, type="response")
table <- table(test$class, predict_rf )
error_rf[i] = sum(diag(table))/sum(table)
}
print("Summary:")
print(paste("SVM mean error:", mean(error)))
print(paste("SVM var error:", var(error)))
print(paste("RF mean error:", mean(error_rf)))
print(paste("RF var error:", var(error_rf)))