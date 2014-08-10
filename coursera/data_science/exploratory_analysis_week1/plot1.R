Sys.setlocale("LC_TIME", "English")
png("plot1.png", width = 480, height = 480)
data = read.csv("household_power_consumption.txt", sep=";", header=T, dec=".", nrows=70000,
                na.strings="?", as.is=T)
data <- transform(data, Date = strptime(paste(Date, Time, sep=" "), "%d/%m/%Y %H:%M:%S"))
data$Time <- NULL
data <- subset(data, (Date > strptime("2007-02-01", "%Y-%m-%d")) & (Date < strptime("2007-02-03", "%Y-%m-%d")))
hist(data$Global_active_power, col = "red", main="Global Active Power", xlab="Global Active Power (kilowatts)")
dev.off()