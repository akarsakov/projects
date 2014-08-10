Sys.setlocale("LC_TIME", "English")
png("plot4.png", width = 480, height = 480)
par(mfrow=c(2,2))
data = read.csv("household_power_consumption.txt", sep=";", header=T, dec=".", nrows=70000,
                na.strings="?", as.is=T)
data <- transform(data, Date = strptime(paste(Date, Time, sep=" "), "%d/%m/%Y %H:%M:%S"))
data$Time <- NULL
data <- subset(data, (Date > strptime("2007-02-01", "%Y-%m-%d")) & (Date < strptime("2007-02-03", "%Y-%m-%d")))

# Global active power vs datetime
plot(data$Date, data$Global_active_power, type="l", ylab="Global Active Power", xlab="")

# Voltage vs datetime
plot(data$Date, data$Voltage, type="l", ylab="Voltage", xlab="datetime")

# Submetering vs datetime
plot(data$Date, data$Sub_metering_1, type="n", ylab="Energy sub metering", xlab="")
lines(data$Date, data$Sub_metering_1, col="black")
lines(data$Date, data$Sub_metering_2, col="red")
lines(data$Date, data$Sub_metering_3, col="blue")
legend("topright", c("Sub_metering_1", "Sub_metering_2", "Sub_metering_3"),
       col = c("black", "red", "blue"), lwd=1)

# Global reactive power vs. datetime
plot(data$Date, data$Global_reactive_power, type="l", xlab="datetime", ylab="Global_reactive_power")
dev.off()