library(e1071);


setwd('D:/Dev/bio/scripts/cl_correct')

cancerData = read.table("metrics_cancer.csv", header=TRUE, sep=',')
cancerData$X <- NULL
cancerWeights = read.table("weights_cancer.csv", header=TRUE, sep=',')
cancerData = cbind(cancerData, cancerWeights)
cancerData["class"] <- "cancer"

healthyData = read.table("metrics_healthy.csv", header=TRUE, sep=',')
healthyData$X <- NULL
healthyWeights = read.table("weights_healthy.csv", header=TRUE, sep=',')
healthyData = cbind(healthyData, healthyWeights)
healthyData["class"] <- "healthy"

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

print("SVM")
svm_model <- svm( train$class ~ . -density -min_weight, train, kernel = "radial", type="C-classification" )
x_test <- subset(test, select = -class);
predy <- predict(svm_model, x_test);
table <- table(test$class, predy );

print(paste("Error 1 (wrong healthy predict):", table[1,2]/sum(table)))
print(paste("Error 2 (wrong cancer predict):", table[2,1]/sum(table)))
print(paste("Error: ", sum(diag(table))/sum(table)))
