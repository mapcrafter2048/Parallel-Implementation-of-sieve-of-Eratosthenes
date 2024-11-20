import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

file_path = '/content/results_openmp_segmented.csv'
data = pd.read_csv(file_path)

num_threads = data['num_threads']
segment_size = data['segment_size']
execution_time = data['Execution_Time']

fig = plt.figure(figsize=(15, 15))
ax = fig.add_subplot(111, projection='3d')

unique_threads = np.unique(num_threads)
unique_segments = np.unique(segment_size)

X, Y = np.meshgrid(unique_threads, np.log10(unique_segments))
Z = np.zeros_like(X, dtype=float)

for i in range(len(unique_threads)):
    for j in range(len(unique_segments)):
        exec_time = data[(data['num_threads'] == unique_threads[i]) &
                         (data['segment_size'] == unique_segments[j])]['Execution_Time']
        Z[j, i] = exec_time.iloc[0] if not exec_time.empty else np.nan

ax.plot_wireframe(X, Y, Z, color='blue', linewidth=1)

ax.set_xlabel('Number of Threads')
ax.set_ylabel('Log10 of Segment Size')
ax.set_zlabel('Execution Time (s)')
ax.set_title('3D Wireframe of Execution Time by Threads and Segment Size (Log Scale)')

plt.show()
