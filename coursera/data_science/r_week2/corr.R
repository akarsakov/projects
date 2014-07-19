corr <- function(directory, threshold = 0) {
    ## 'directory' is a character vector of length 1 indicating
    ## the location of the CSV files
    
    ## 'threshold' is a numeric vector of length 1 indicating the
    ## number of completely observed observations (on all
    ## variables) required to compute the correlation between
    ## nitrate and sulfate; the default is 0
    
    ## Return a numeric vector of correlations
    
    cors = c()
    for (file_name in list.files(directory))
    {
        monitor_data = read.csv(paste(directory, file_name, sep="/"))
        nobs = sum(complete.cases(monitor_data))
        if (nobs > threshold)
        {
            compl = monitor_data[complete.cases(monitor_data),]
            cors = c(cors, cor(compl$sulfate, compl$nitrate))
        }
    }
    cors
}