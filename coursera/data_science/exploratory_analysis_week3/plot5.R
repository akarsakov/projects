library(ggplot2)
NEI <- readRDS("summarySCC_PM25.rds")
SCC <- readRDS("Source_Classification_Code.rds")

# get motor vehicle source ids
motor_sources_ids <- SCC[grep("*[vV]ehicle*", SCC$Short.Name), "SCC"]

# prepare dataset
NEI_motor <- NEI[NEI$SCC %in% motor_sources_ids & NEI$fips == "24510",]
NEI_motor <- transform(NEI_motor, year=factor(year), type=factor(type))
data <- aggregate(Emissions ~ year, data=NEI_motor, sum)
colnames(data) <- c("Year", "Emissions")

# plot dataset
png("plot5.png", width = 640, height = 480)
g <- ggplot(data, aes(Year, Emissions, group=1))
g + geom_point() + geom_smooth(method = "lm") + ylab("Amount of PM2.5 emitted (tons)") +
    ggtitle("Total emissions from motor vehicle sources in the Baltimore City, Maryland (1999-2008)")
dev.off()