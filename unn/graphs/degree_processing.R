setwd("D:/Dev/bio/scripts/degree/full/BRCA")

plot_line <- function(start_x, end_x, a, c, x_offset)
{
  xx = vector(mode="numeric", length=num_steps+1)
  yy = vector(mode="numeric", length=num_steps+1)
  for (i in 1:(num_steps+1))
  {
    xx[i] = (end_x - start_x)*(i-1)/num_steps + start_x
    yy[i] = a*xx[i] + c
    xx[i] = xx[i] + x_offset
  }
  lines(xx,yy,col="green",lwd=2)
}

healthy_data = scan("degree_healthy_4.csv")
cancer_data = scan("degree_cancer_1.csv")
num_steps = 1000

hist(log10(cancer_data), breaks=100, xlim=c(0.75, 3.1), main="Гистограмма распределения степеней вершин для графа больного пациента",
     xlab="log10(deg(v))", ylab="Частота")
hist(log10(healthy_data), breaks=100, xlim=c(0.75, 3.1), main="Гистограмма распределения степеней вершин для графа здорового пациента",
     xlab="log10(deg(v))", ylab="Частота")

max_val = max(max(healthy_data), max(cancer_data))
min_val = min(min(healthy_data), min(cancer_data))
x = vector(mode="numeric", length=num_steps+1)
distrib = vector(mode="numeric", length=num_steps+1)

for(i in 1:(num_steps+1))
{
  x[i] = min_val + (max_val - min_val)*(i-1)/num_steps
  distrib[i] = length(healthy_data[healthy_data >= x[i]])/length(healthy_data)
}

plot(log10(x), log10(distrib), type='l', col="red", ylim=c(-5, 0.5), xlab="log10(avg degree)", ylab="log10(% vertex)")

x_data = log10(x)
y_data = log10(distrib)

dataset = as.data.frame(cbind(x_data, y_data))
colnames(dataset) <- c("x", "y")
is.na(dataset) <- sapply(dataset, is.infinite)
dataset = dataset[complete.cases(dataset),]

start_x = 1.8
end_x = 3

dataset = dataset[dataset$x > start_x, ]
dataset = dataset[dataset$x < end_x, ]

model = lm(y ~ x, dataset)
plot_line(start_x, end_x, model$coefficients[2], model$coefficients[1], 0) 
plot_line(start_x, end_x, model$coefficients[2], model$coefficients[1], 0.17) 


for(i in 1:(num_steps+1))
{
  distrib[i] = length(cancer_data[cancer_data >= x[i]])/length(cancer_data)
}
lines(log10(x), log10(distrib), col="blue")

legend("topright", c("cancer", "healthy", paste("fit healthy y=c + (", round(model$coefficients[2],3), ")*x", sep="")), col=c("blue", "red", "green"), pch = 1)
#legend("bottomright", c("cancer", "healthy"), col=c("blue", "red"), pch = 1)
