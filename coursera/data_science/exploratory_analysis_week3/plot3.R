NEI <- readRDS("summarySCC_PM25.rds")

data <- NEI[NEI$fips == "24510",]
data <- lapply(split(NEI_Baltimore$Emissions, as.factor(NEI_Baltimore$year)), sum)
#png("plot3.png", width = 640, height = 480)
plot(names(data), data, type="o", main="Total emissions from PM2.5 in the Baltimore City, Maryland from 1999 to 2008",
     xlab="Year", ylab="Amount of PM2.5 emitted (tons)", xaxt="n")
axis(1, at = seq(1999, 2008, by = 3), las=1)
#dev.off()