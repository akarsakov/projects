library(igraph)

num_graphs=19

setwd('D:/Dev/bio/scripts/graphs')
cancerGraphMetrics = as.data.frame(array(0, dim=c(num_graphs, 9)))
colnames(cancerGraphMetrics) <- c("diameter", "evcent", "degcent", "clcent", "betwcent", "density", "transitivity", 
                                  "avg_path", "clusters")
for (i in 1:num_graphs)
{
  g = read.graph(paste("cancer_", i-1, ".csv", sep=""), format="edgelist")
  print(i)
  cancerGraphMetrics[i, 1] = diameter(g)
  cancerGraphMetrics[i, 2] = centralization.evcent(g)$centralization
  cancerGraphMetrics[i, 3] = centralization.degree(g)$centralization
  cancerGraphMetrics[i, 4] = centralization.closeness(g)$centralization
  cancerGraphMetrics[i, 5] = centralization.betweenness(g)$centralization
  cancerGraphMetrics[i, 6] = graph.density(g, loops=FALSE)
  cancerGraphMetrics[i, 7] = transitivity(g)
  cancerGraphMetrics[i, 8] = average.path.length(g)
  cancerGraphMetrics[i, 9] = length(clusters(g)$csize)
}

write.csv(cancerGraphMetrics, file="metrics.csv")
