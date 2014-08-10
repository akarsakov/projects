Sys.setlocale("LC_TIME", "English")
png("plot3.png", width = 480, height = 480)
data = read.csv("household_power_consumption.txt", sep=";", header=T, dec=".", nrows=70000,
                na.strings="?", as.is=T)
data <- transform(data, Date = strptime(paste(Date, Time, sep=" "), "%d/%m/%Y %H:%M:%S"))
data$Time <- NULL
data <- subset(data, (Date > strptime("2007-02-01", "%Y-%m-%d")) & (Date < strptime("2007-02-03", "%Y-%m-%d")))
plot(data$Date, data$Sub_metering_1, type="n", ylab="Energy sub metering", xlab="")
lines(data$Date, data$Sub_metering_1, col="black")
lines(data$Date, data$Sub_metering_2, col="red")
lines(data$Date, data$Sub_metering_3, col="blue")
legend("topright", c("Sub_metering_1", "Sub_metering_2", "Sub_metering_3"),
       col = c("black", "red", "blue"), lwd=1)
dev.off()