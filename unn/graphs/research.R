setwd("D:/Dev/bio/scripts/degree/full")

plot_data <- function(gene_i, gene_j)
{
  min_x = min(min(train_data[,gene_i]), min(min(test_cancer[,gene_i]), min(test_healthy[,gene_i])))
  max_x = max(max(train_data[,gene_i]), max(max(test_cancer[,gene_i]), max(test_healthy[,gene_i])))
  
  min_y = min(min(train_data[,gene_j]), min(min(test_cancer[,gene_j]), min(test_healthy[,gene_j])))
  max_y = max(max(train_data[,gene_j]), max(max(test_cancer[,gene_j]), max(test_healthy[,gene_j])))
  
  #plot(train_data[,gene_i], train_data[,gene_j], pch = 20, col="green", xlim=c(min_x, max_x), ylim=c(min_y, max_y), xlab=names[gene_i], ylab=names[gene_j])
  plot(train_data[,gene_i], train_data[,gene_j], pch = 20, col="green", xlim=c(min_x, max_x), ylim=c(min_y, max_y), xlab="ZFP106", ylab="TRIM9")
  
  points(test_cancer[,gene_i], test_cancer[,gene_j], pch = 20, col="red")
  #points(test_cancer[cancer_case,gene_i], test_cancer[cancer_case, gene_j], col="blue", pch=8)
  
  points(test_healthy[,gene_i], test_healthy[,gene_j], pch = 20, col="blue")
  #points(test_healthy[healthy_case,gene_i], test_healthy[healthy_case, gene_j], col="green", pch=8)
  
  formula = as.formula(paste(names[gene_j], "~", names[gene_i]))
  model = lm(formula, train_data)
  
  x = vector(mode="numeric", length=1000)
  y = vector(mode="numeric", length=1000)
  for (i in 1:1000)
  {
    x[i] = (max_x - min_x)*i/1000 + min_x
    y[i] = model$coefficients[1] + model$coefficients[2]*x[i]
  }
  lines(x,y)
}

train_data = read.csv("BRCA_train.txt")
print("train loaded")
test_cancer = read.csv("BRCA_test_cancer.txt")
print("test_cancer loaded")
test_healthy = read.csv("BRCA_test_healthy.txt")
print("test_healthy loaded")

cancer_case = 1
healthy_case = 1


gene_i = 15279
gene_j = 2689

names = colnames(train_data)

plot_data(gene_i, gene_j)

legend("topleft", c("Контрольная выборка", "Здоровые пациенты", "Больные пациенты", "Аппроксимирующая прямая"), col=c("green", "blue", "red", "black"), pch = 1)
