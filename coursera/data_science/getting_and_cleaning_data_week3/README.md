### Course project for "Getting and cleaning data"
##### Content:
- run_analysis.R - script for generating tidy data from full data sets
- codebook.txt - description of each variables in resulting data set
- README.md - this file

##### How to use script:
1. Put dataset from http://archive.ics.uci.edu/ml/datasets/Human+Activity+Recognition+Using+Smartphones# to working directory.
2. Run `run_analysis.R` script.
3. Resulting dataset will be stored in `tidy_data.txt` file.

`run_analysis.R` script performs following steps:

1. Merges the training and the test sets to create one data set.
2. Extracts only the measurements on the mean and standard deviation for each measurement. 
3. Add descriptive activity names to name the activities and appropriately labels for variables. 
4. Creates tidy data set with the average of each variable for each activity and each subject. 

##### Remarks
- If you are interested how `run_analysis.R` script works feel free to inspect the code. Script contains a lot of describing comments.
- I prefer `UpperCamelCase` naming convension for names of variables.
