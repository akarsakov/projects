rankhospital <- function(state, outcome, num = "best") {
    ## Read outcome data
    ## Check that state and outcome are valid
    ## Return hospital name in that state with the given rank
    ## 30-day death rate
    outcome_data <- read.csv("data/outcome-of-care-measures.csv", na.strings = "Not Available", dec=".", sep=",", as.is=T)
    states = unique(outcome_data$State)
    if (!(state %in% states)) {
        stop("invalid state")
    }
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
    
    data <- outcome_data[outcome_data$State == state, c(2, col)]
    data <- data[complete.cases(data),]
    if (num == "best") {
        data <- data[order(data[,2]), ]
        data[1,1]
    } else if (num == "worst") {
        data <- data[order(-data[,2]), ]
        data[1,1]
    } else {
        data <- data[order(data[,2]), ]
        data[num,1]
    }
}
