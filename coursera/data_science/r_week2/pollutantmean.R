pollutantmean <- function(directory, pollutant, id = 1:332) {
    ## 'directory' is a character vector of length 1 indicating
    ## the location of the CSV files
    
    ## 'pollutant' is a character vector of length 1 indicating
    ## the name of the pollutant for which we will calculate the
    ## mean; either "sulfate" or "nitrate".
    
    ## 'id' is an integer vector indicating the monitor ID numbers
    ## to be used
    
    ## Return the mean of the pollutant across all monitors list
    ## in the 'id' vector (ignoring NA values)
    
    temp_vector = c()
    for (monitor_id in id)
    {
        monitor_data = read.csv(paste(directory, "/", sprintf("%03d.csv", monitor_id), sep=""))
        temp_vector = c(temp_vector, monitor_data[,pollutant])
    }
    mean(temp_vector, na.rm=T) 
}