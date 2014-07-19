complete <- function(directory, id = 1:332) {
    ## 'directory' is a character vector of length 1 indicating
    ## the location of the CSV files
    
    ## 'id' is an integer vector indicating the monitor ID numbers
    ## to be used
    
    ## Return a data frame of the form:
    ## id nobs
    ## 1  117
    ## 2  1041
    ## ...
    ## where 'id' is the monitor ID number and 'nobs' is the
    ## number of complete cases
    
    nobs = c()
    for (monitor_id in id)
    {
        monitor_data = read.csv(paste(directory, "/", sprintf("%03d.csv", monitor_id), sep=""))
        nobs = c(nobs, sum(complete.cases(monitor_data)))
    }
    as.data.frame(cbind(id, nobs))
}