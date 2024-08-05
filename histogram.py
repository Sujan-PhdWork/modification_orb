import matplotlib.pyplot as plt
import numpy as np

# File path
file_path = 'evaluation/histogram/evaluation_bonn_c2.txt'

# Reading data from file
allvalues = []
#values = []
with open(file_path, 'r') as file:
    for line in file:
        # Extracting the first value after the colon
        #first_value = float(line.split(':')[1].split(',')[0].strip())
        print(line)
        values = [float(value.strip()) for value in line.split(':')[0].split(',')]
        allvalues.append(values)     
        #values.append(first_value)
allvalues=np.array(allvalues)
median_values = np.median(allvalues,axis=0)
#values=np.array(values)
#median_value = np.median(values)
print(median_values)
# Plotting histogram
#plt.hist(values, bins=25, edgecolor='black')
#plt.axvline(median_value, color='r', linestyle='solid', linewidth=3)
#plt.title('Histogram of ATE from ORBSLAM2 on X')
#plt.xlim(0.013,0.017)
#plt.ylim(0,5)
#plt.xlabel('ATE(m)')
#plt.ylabel('Frequency')
#plt.grid(True)

# Show the plot
#plt.show()
