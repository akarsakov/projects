NEI <- readRDS("summarySCC_PM25.rds")

# prepare dataset
NEI_Baltimore <- NEI[NEI$fips == "24510",]
data <- aggregate(Emissions ~ year, data=NEI_Baltimore, sum)

# plot dataset
png("plot2.png", width = 640, height = 480)
plot(data$year, data$Emissions, type="o", main="Total emissions from PM2.5 in the Baltimore City, Maryland from 1999 to 2008",
     xlab="Year", ylab="Amount of PM2.5 emitted (tons)", xaxt="n")
axis(1, at = seq(1999, 2008, by = 3), las=1)
dev.off()