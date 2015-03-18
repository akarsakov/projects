setwd('D:/Dev/bio/scripts/')
data = as.data.frame(read.table("out.csv", header=TRUE, sep=','))

cancerData = data[1:100, ] #data[data$class == "cancer",]
healthData = data[data$class == "healthy",]

index <- sample(1:nrow(healthData), 70)
trainHealthData = healthData[-index,]
testHealthData = healthData[index,]

size = nrow(healthData)

index <- sample(1:size, 70)
trainHalthData = healthData[index,]
testHealthData = healthData[-index,]

trainHealthData$class <- NULL
size = ncol(trainHealthData)
name = names(trainHealthData)

results = array(0, dim=c(nrow(cancerData), size, size))
testResults = array(0, dim=c(nrow(testHealthData), size, size))
name = names(healthData)

for (i in 1:(size-1))
{
  print(i)
  for (j in (i+1):size)
  {
    print(j)
    formula = as.formula(paste(name[i], " ~ ", name[j]))
    print(formula)
    model = lm(formula, data=trainHealthData)
    # calculate on cancer data
    pr = predict(model, cancerData)
    v = var(trainHealthData[, name[i]])
    results[,i,j] = abs(pr - cancerData[, name[i]])/v
    results[,j,i] = results[,i,j]
    #calculate on healthy data
    pr = predict(model, testHealthData)
    testResults[,i,j] = abs(pr - testHealthData[, name[i]])/v
    testResults[,j,i] = testResults[,i,j]
  }
  print(".")
}
print("finish")
print("write cancer data")
for (i in 1:nrow(cancerData))
{
  temp = results[i,,]
  colnames(temp) <- name
  rownames(temp) <- name
  write.csv(temp, file=paste("cancer", i, ".csv", sep=""))
}
print("write healthy data")
for (i in 1:nrow(testHealthData))
{
  temp = testResults[i,,]
  colnames(temp) <- name
  rownames(temp) <- name
  write.csv(temp, file=paste("healthy", i, ".csv", sep=""))
}