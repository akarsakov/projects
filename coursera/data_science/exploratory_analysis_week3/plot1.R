NEI <- readRDS("summarySCC_PM25.rds")

data <- lapply(split(NEI$Emissions, as.factor(NEI$year)), sum)
png("plot1.png", width = 480, height = 480)
plot(names(data), data, type="o", main="Total emissions from PM2.5 in the US from 1999 to 2008",
     xlab="Year", ylab="Amount of PM2.5 emitted (tons)", xaxt="n"))
axis(1, at = seq(1999, 2008, by = 3), las=1)
dev.off()