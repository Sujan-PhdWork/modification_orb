import matplotlib.pyplot as plt
import numpy as np

# File path
file_path = 'evaluation_results.txt'

# Reading data from file
allvalues = []
with open(file_path, 'r') as file:
    for line in file:
        # Extracting the first value after the colon
        #first_value = float(line.split(':')[1].split(',')[0].strip())
        values = [float(value.strip()) for value in line.split(':')[1].split(',')]
        allvalues.append(values)     
        #values.append(first_value)
allvalues=np.array(allvalues)
median_values = np.median(allvalues,axis=0)
print(median_values)
# # Plotting histogram
# plt.hist(values, bins=25, edgecolor='black')
# plt.axvline(median_value, color='r', linestyle='dashed', linewidth=1)
# plt.title('Histogram of First Values from SLAM Evaluation Results')
# plt.xlim(0.013,0.017)
# plt.xlabel('Values')
# plt.ylabel('Frequency')
# plt.grid(True)

# # Show the plot
# plt.show()
