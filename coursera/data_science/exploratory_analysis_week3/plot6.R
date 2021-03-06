library(ggplot2)
NEI <- readRDS("summarySCC_PM25.rds")
SCC <- readRDS("Source_Classification_Code.rds")
city_labels <- c("Los Angeles County, California", "Baltimore City, Maryland")

# get motor vehicle source ids
motor_sources_ids <- SCC[grep("*[vV]ehicle*", SCC$Short.Name), "SCC"]

# prepare dataset
NEI_motor <- NEI[NEI$SCC %in% motor_sources_ids & (NEI$fips == "24510" | NEI$fips == "06037"),]
NEI_motor <- transform(NEI_motor, year=factor(year), type=factor(type), 
                       fips=factor(fips, labels=city_labels))
data <- aggregate(Emissions ~ year + fips, data=NEI_motor, sum)
colnames(data) <- c("Year", "fips", "Emissions")

# plot dataset
png("plot6.png", width = 640, height = 480)
g <- ggplot(data, aes(Year, Emissions, group=1))
g + geom_point() + 
    facet_grid(. ~ fips) + 
    geom_smooth(method = "lm") +
    ggtitle("Total emissions from motor vehicle in Los Angeles County, California 
            and Baltimore City, Maryland (1999-2008)")
dev.off()
