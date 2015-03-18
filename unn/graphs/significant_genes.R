library(igraph)

setwd('D:/Dev/bio/scripts/')
writeFile = TRUE
max_edges = 500
max_vertex = 4

data = read.table("out.csv", header=TRUE, sep=',')

cancerData = data[1:100, ] #data[data$class == "cancer",]
healthData = data[data$class == "healthy",]

index <- sample(1:nrow(healthData), 50)
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
graphMetrics = as.data.frame(array(0, dim=c(nrow(testHealthData) + nrow(cancerData), 3)))
cancerVertex = as.data.frame(array(0, dim=c(length(name))))
healthVertex = as.data.frame(array(0, dim=c(length(name))))
for (i in 1:nrow(cancerData))
{
  adj_mat = cancerResults[i,,]
  temp = sort(as.vector(adj_mat), decreasing = TRUE)
  threshold = temp[2*max_edges + 1]
  adj_mat = adj_mat > threshold
  g = graph.adjacency(adj_mat)
  graphMetrics[i,] = c("cancer", diameter(g), centralization.evcent(g)$centralization)
  # vertex degree
  degrees = degree(g)
  temp = sort(as.vector(degrees), decreasing = TRUE)
  threshold = temp[max_vertex+1]
  temp = degrees > threshold
  cancerVertex = cancerVertex + temp
  if (writeFile)
  {
    colnames(adj_mat) <- name
    rownames(adj_mat) <- name
    write.csv(adj_mat, file=paste("cancer", i, ".csv", sep=""))
  }
}
print("write healthy data")
for (i in 1:nrow(testHealthData))
{
  adj_mat = healthResults[i,,]
  temp = sort(as.vector(adj_mat), decreasing = TRUE)
  threshold = temp[2*max_edges + 1]
  adj_mat = adj_mat > threshold
  g = graph.adjacency(adj_mat)
  graphMetrics[i + nrow(cancerData),] = c("healthy", diameter(g), centralization.evcent(g)$centralization)
  # vertex degree
  degrees = degree(g)
  temp = sort(as.vector(degrees), decreasing = TRUE)
  threshold = temp[max_vertex+1]
  temp = degrees > threshold
  healthVertex = healthVertex + temp
  if (writeFile)
  {
    colnames(adj_mat) <- name
    rownames(adj_mat) <- name
    write.csv(adj_mat, file=paste("healthy", i, ".csv", sep=""))
  }
}

print("print first 10 significant genes")
print("for cancer:")
temp = sort(cancerVertex[,], decreasing = TRUE)
threshold = temp[11]
for (i in 1:length(cancerVertex[,]))
{  
  if (cancerVertex[i,] >= threshold)
  {
    print(paste(name[i], " - ", cancerVertex[i,]))
  }
}

print("for healthy:")
temp = sort(healthVertex[,], decreasing = TRUE)
threshold = temp[11]
for (i in 1:length(healthVertex[,]))
{  
  if (healthVertex[i,] >= threshold)
  {
    print(paste(name[i], " - ", healthVertex[i,]))
  }
}

write.csv(graphMetrics, file="graph_metrics.csv")