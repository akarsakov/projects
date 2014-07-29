best <- function(state, outcome)
{
    ## Read outcome data
    ## Check that state and outcome are valid
    ## Return hospital name in that state with lowest 30-day death
    ## rates
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
    best <- data[data[,2] == min(data[,2]), ]
    best <- best[order(best[,1]), ]
    best[1,1]
}