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
EVALUATION_RESULTS="evaluation_results_rpe.txt"

# Temporary files to store individual results
TMP_tRMSE="tmp_trmse.txt"
TMP_tSTD="tmp_tStd.txt"
TMP_RRMSE="tmp_Rrmse.txt"
TMP_RSTD="tmp_Rstd.txt"

# Clear the output file if it already exists
> $EVALUATION_RESULTS

# Clear temporary files
> $TMP_tRMSE
> $TMP_tSTD
> $TMP_RRMSE
> $TMP_RSTD

# Loop over the result files
for result_file in $RESULT_DIR/*.txt
do
    # Extract the file number (e.g., 1, 2, 3, etc.)
    file_number=$(basename $result_file .txt)
    
    # Run the python command and capture the output
    output=$(python2.7 evaluate_rpe.py $GROUND_TRUTH_FILE $result_file)
    # Extract values from the output
    t_rmse=$(echo $output | cut -d',' -f1 | xargs) # rmse value
    t_std=$(echo $output | cut -d',' -f2 | xargs) # std value
    r_rmse=$(echo $output | cut -d',' -f3 | xargs) # rmse value
    r_std=$(echo $output | cut -d',' -f4 | xargs) # std value
    
    # Append the output to the evaluation results file with the file numbers
    #echo "Results${file_number}.txt: $output" >> $EVALUATION_RESULTS
    echo "${file_number}: $output" >> $EVALUATION_RESULTS
    # Append the values to the temporary files
    echo $t_rmse >> $TMP_tRMSE
    echo $t_std >> $TMP_tSTD
    echo $r_rmse >> $TMP_RRMSE
    echo $r_std >> $TMP_RSTD
done

compute_median() {
    sort -n $1 | awk '{ a[i++]=$1; } END { if (i % 2) { print a[int(i/2)]; } else { print (a[int(i/2)-1] + a[int(i/2)]) / 2; } }'
}

median_rmse=$(compute_median $TMP_tRMSE)
median_mean=$(compute_median $TMP_tSTD)
median_median=$(compute_median $TMP_RRMSE)
median_std=$(compute_median $TMP_RSTD)

# Compute the mean of rmse, mean, median, and std
#mean_rmse=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_RMSE)
#mean_mean=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_MEAN)
#mean_median=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_MEDIAN)
#mean_std=$(awk '{sum+=$1} END {if (NR>0) print sum / NR}' $TMP_STD)

# Append the averages to the evaluation results file
#echo -e "\nMean RMSE: $mean_rmse" >> $EVALUATION_RESULTS
#echo "Mean Mean: $mean_mean" >> $EVALUATION_RESULTS
#echo "Mean Median: $mean_median" >> $EVALUATION_RESULTS
#echo "Mean Std: $mean_std" >> $EVALUATION_RESULTS

echo -e "\nMedian RMSE: $median_rmse"
echo "Median Mean: $median_mean"
echo "Median Median: $median_median"
echo "Median Std: $median_std"


rm $TMP_tRMSE
rm $TMP_tSTD
rm $TMP_RRMSE
rm $TMP_RSTD

