#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <result_directory> <ground_truth_file>"
    exit 1
fi

# Assign the arguments to variables
RESULT_DIR="$1"
GROUND_TRUTH_FILE="$2"
#RESULT_DIR="result"

# Define the output file for storing the evaluation results
EVALUATION_RESULTS="evaluation_results.txt"

# Temporary files to store individual results
TMP_RMSE="tmp_rmse.txt"
TMP_MEAN="tmp_mean.txt"
TMP_MEDIAN="tmp_median.txt"
TMP_STD="tmp_std.txt"

# Clear the output file if it already exists
> $EVALUATION_RESULTS

# Clear temporary files
> $TMP_RMSE
> $TMP_MEAN
> $TMP_MEDIAN
> $TMP_STD

# Loop over the result files
for result_file in $RESULT_DIR/*.txt
do
    # Extract the file number (e.g., 1, 2, 3, etc.)
    file_number=$(basename $result_file .txt)
    
    # Run the python command and capture the output
    output=$(python2.7 evaluate_ate.py $GROUND_TRUTH_FILE $result_file)
    
    # Extract values from the output
    rmse=$(echo $output | cut -d',' -f1 | xargs) # rmse value
    mean=$(echo $output | cut -d',' -f2 | xargs) # mean value
    median=$(echo $output | cut -d',' -f3 | xargs) # median value
    std=$(echo $output | cut -d',' -f4 | xargs) # std value
    
    # Append the output to the evaluation results file with the file numbers
    #echo "Results${file_number}.txt: $output" >> $EVALUATION_RESULTS
    echo "${file_number}: $output" >> $EVALUATION_RESULTS
    # Append the values to the temporary files
    echo $rmse >> $TMP_RMSE
    echo $mean >> $TMP_MEAN
    echo $median >> $TMP_MEDIAN
    echo $std >> $TMP_STD
done

# Compute the mean of rmse, mean, median, and std
mean_rmse=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_RMSE)
mean_mean=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_MEAN)
mean_median=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_MEDIAN)
mean_std=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_STD)

# Append the averages to the evaluation results file
#echo -e "\nMean RMSE: $mean_rmse" >> $EVALUATION_RESULTS
#echo "Mean Mean: $mean_mean" >> $EVALUATION_RESULTS
#echo "Mean Median: $mean_median" >> $EVALUATION_RESULTS
#echo "Mean Std: $mean_std" >> $EVALUATION_RESULTS

echo -e "\nMean RMSE: $mean_rmse" 
echo "Mean Mean: $mean_mean" 
echo "Mean Median: $mean_median"
echo "Mean Std: $mean_std"

