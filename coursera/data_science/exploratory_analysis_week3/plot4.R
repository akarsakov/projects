library(ggplot2)
NEI <- readRDS("summarySCC_PM25.rds")
SCC <- readRDS("Source_Classification_Code.rds")

# get coal combustion-related source ids
coal_sources_ids <- SCC[grep("[Cc]oal", SCC$Short.Name), "SCC"]

# prepare dataset
NEI_coal <- NEI[NEI$SCC %in% coal_sources_ids,]
NEI_coal <- transform(NEI_coal, year=factor(year), type=factor(type))
data <- aggregate(Emissions ~ year, data=NEI_coal, sum)
colnames(data) <- c("Year", "Emissions")
# plot dataset
png("plot4.png", width = 640, height = 480)
g <- ggplot(data, aes(Year, Emissions, group=1))
g + geom_point() + geom_smooth(method = "lm") + ylab("Amount of PM2.5 emitted (tons)") +
    ggtitle("Total emissions from coal combustion-related sources in the US (1999-2008)")
dev.off()