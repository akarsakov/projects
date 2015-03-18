library(igraph)
library(e1071)

setwd('D:/Dev/bio/scripts/')
writeFile = FALSE

data = read.table("out.csv", header=TRUE, sep=',')

cancerData = data[1:100, ] #data[data$class == "cancer",]
healthData = data[data$class == "healthy",]

index <- sample(1:nrow(healthData), 70)
trainHealthData = healthData[index,]
testHealthData = healthData[-index,]

trainHealthData$class <- NULL
size = ncol(trainHealthData)
cancerResults = array(0, dim=c(nrow(cancerData), size, size))
healthResults = array(0, dim=c(nrow(testHealthData), size, size))
name = names(trainHealthData)
print("calculate weight of edges")
for (i in 1:(size-1))
{
  for (j in (i+1):size)
  {
    formula = as.formula(paste(name[i], " ~ ", name[j]))
    #print(formula)
    model = lm(formula, data=trainHealthData)
    # calculate on cancer data
    pr = predict(model, cancerData)
    v = var(healthData[, name[i]])
    cancerResults[,i,j] = abs(pr - cancerData[, name[i]])/v
    cancerResults[,j,i] = cancerResults[,i,j]
    #calculate on healthy data
    pr = predict(model, testHealthData)
    healthResults[,i,j] = abs(pr - testHealthData[, name[i]])/v
    healthResults[,j,i] = healthResults[,i,j]
  }
  print(i)
}
print("finished!")
print("process cancer data")
cancerGraphMetrics = as.data.frame(array(0, dim=c(nrow(cancerData), 3)))
healthGraphMetrics = as.data.frame(array(0, dim=c(nrow(testHealthData), 3)))
cancerVertex = as.data.frame(array(0, dim=c(length(name))))
healthVertex = as.data.frame(array(0, dim=c(length(name))))
for (i in 1:nrow(cancerData))
{
  adj_mat = cancerResults[i,,]
  degrees = rowSums(adj_mat)
  cancerVertex = cancerVertex + degrees
}
print("write healthy data")
for (i in 1:nrow(testHealthData))
{
  adj_mat = healthResults[i,,]
  degrees = rowSums(adj_mat)
  healthVertex = healthVertex + degrees
}

cancerVertex = cancerVertex / nrow(cancerData)
healthVertex = healthVertex / nrow(testHealthData)

print("for cancer:")
res = cbind(name, cancerVertex)
names(res) <- c("name", "average degree")
res = res[order(-res[,2]),]
write.csv(res, file="cancer_genes2.csv")

print("for helthy:")
res = cbind(name, healthVertex)
names(res) <- c("name", "average degree")
res = res[order(-res[,2]),]
write.csv(res, file="health_genes2.csv")