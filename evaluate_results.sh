#!/bin/bash

# Define the result directory
RESULT_DIR="result"

# Define the output file
OUTPUT_FILE="evaluation_results.txt"

# Create or clear the output file
> $OUTPUT_FILE

# Temporary file to store the floating-point outputs
TEMP_FILE="temp_outputs.txt"
> $TEMP_FILE



# Loop through each text file in the result directory
for file in $RESULT_DIR/*.txt
do
    # Extract the base name of the file (e.g., 1.txt)
    base_name=$(basename $file)
    
    # Run the evaluation command and capture the output
    output=$(python2.7 evaluate_ate.py ../dataset/rgbd_dataset_freiburg3_walking_xyz_validation/groundtruth.txt $file)
    
    # Save the output to the output file and the temp file
    echo "$base_name: $output" >> $OUTPUT_FILE
    echo $output >> $TEMP_FILE
done

# Calculate the mean and standard deviation using awk
mean=$(awk '{sum+=$1} END {print sum/NR}' $TEMP_FILE)
stddev=$(awk '{x[NR]=$1; sum+=$1} END {mean=sum/NR; for (i=1;i<=NR;i++){sumsq+=(x[i]-mean)^2} print sqrt(sumsq/NR)}' $TEMP_FILE)

median=$(sort -n $TEMP_FILE | awk '{a[NR]=$1} END {if (NR%2==1) {print a[(NR+1)/2]} else {print (a[NR/2] + a[NR/2+1])/2}}')

# Append the mean, median, and standard deviation to the output file
echo "Mean: $mean" >> $OUTPUT_FILE
echo "Median: $median" >> $OUTPUT_FILE
echo "Standard Deviation: $stddev" >> $OUTPUT_FILE

# Print the mean, median, and standard deviation to the terminal
echo "Mean: $mean"
echo "Median: $median"
echo "Standard Deviation: $stddev"

# Remove the temporary file
rm $TEMP_FILE

