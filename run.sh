#!/bin/bash

# Define the number of iterations
NUM_ITERATIONS=10

# Define the result directory
RESULT_DIR="result"

# Create the result directory if it doesn't exist
mkdir -p $RESULT_DIR

rm -rf $RESULT_DIR/* 

# Loop for the specified number of iterations
for ((i=1; i<=NUM_ITERATIONS; i++))
do
    # Run the command
    Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt Examples/RGB-D/bonn.yaml ../dataset/bonn/rgbd_bonn_balloon/ Examples/RGB-D/associations/bonn_balloon.txt
    
    # Copy and rename the CameraTrajectory.txt file
    cp CameraTrajectory.txt $RESULT_DIR/${i}.txt
done

