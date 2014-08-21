library(ggplot2)
NEI <- readRDS("summarySCC_PM25.rds")

# prepare dataset
NEI_Baltimore <- NEI[NEI$fips == "24510",]
NEI_Baltimore <- transform(NEI_Baltimore, year=factor(year), type=factor(type))
data <- aggregate(Emissions ~ year + type, data=NEI_Baltimore, sum)
colnames(data) <- c("Year", "Type", "Emissions")
data <- transform(data, type=factor(Type))

# plot dataset
png("plot3.png", width = 640, height = 480)
g <- ggplot(data, aes(Year, Emissions, group=1))
g + geom_point() + ylab("Amount of PM2.5 emitted (tons)") +
    facet_grid(. ~ type) + scale_y_continuous(limit = c(0, 2500)) +
    geom_smooth(method = "lm", fill=NA) +
    ggtitle("Total emissions in the Baltimore City, Maryland by type of source (1999-2008)")
dev.off()
