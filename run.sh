#!/bin/bash

# Define the number of iterations
NUM_ITERATIONS=50

# Define the result directory
RESULT_DIR="result"

# Create the result directory if it doesn't exist
mkdir -p $RESULT_DIR

# Loop for the specified number of iterations
for ((i=1; i<=NUM_ITERATIONS; i++))
do
    # Run the command
    ./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt Examples/RGB-D/TUM3.yaml ../dataset/rgbd_dataset_freiburg3_walking_halfsphere Examples/RGB-D/associations/fr3_walking_halfsphere.txt 
    
    # Copy and rename the CameraTrajectory.txt file
    cp CameraTrajectory.txt $RESULT_DIR/${i}.txt
done

