rankall <- function(outcome, num = "best") {
    ## Read outcome data
    ## Check that state and outcome are valid
    ## For each state, find the hospital of the given rank
    ## Return a data frame with the hospital names and the
    ## (abbreviated) state name
    outcome_data <- read.csv("data/outcome-of-care-measures.csv", na.strings = "Not Available", dec=".", sep=",", as.is=T)
    states <- sort(unique(outcome_data$State))
    outcomes <- c("heart attack", "heart failure", "pneumonia")
    if (!(outcome %in% outcomes))
    {
        stop("invalid outcome")
    }
    
    col <- 0
    if (outcome == "heart attack") {
        col <- 11
    } else if (outcome == "heart failure") {
        col <- 17
    } else if (outcome == "pneumonia") {
        col <- 23
    }
    ranks <- c()    
    for (state in states)
    {
        data <- outcome_data[outcome_data$State == state, c(2, col)]
        data <- data[complete.cases(data),]
        if (num == "best") {
            data <- data[order(data[,2],data[,1]), ]
            ranks <- c(ranks, data[1,1])
        } else if (num == "worst") {
            data <- data[order(-data[,2],data[,1]), ]
            ranks <- c(ranks, data[1,1])
        } else {
            data <- data[order(data[,2],data[,1]), ]
            ranks <- c(ranks, data[num,1])
        }
    }
    result <- data.frame(x = ranks, y = states, row.names=states)
    colnames(result) <- c("hospital", "state")
    result
}
