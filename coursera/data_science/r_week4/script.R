setwd("D:/Dev/git_repos/projects/coursera/data_science/r_week4")

print("================ tests for 'best' ================")
source("best.R")
print(best("TX", "heart attack"))
print(best("TX", "heart failure"))
print(best("MD", "heart attack"))
print(best("MD", "pneumonia"))
#print(best("BB", "heart attack"))
#print(best("NY", "hert attack"))


print("============ tests for 'rankhospital' ============")
source("rankhospital.R")
print(rankhospital("TX", "heart failure", 4))
print(rankhospital("MD", "heart attack", "worst"))
print(rankhospital("MN", "heart attack", 5000))

print("=============== tests for 'rankall' ==============")
source("rankall.R")
print(head(rankall("heart attack", 20), 10))
print(tail(rankall("pneumonia", "worst"), 3))
print(tail(rankall("heart failure"), 10))
