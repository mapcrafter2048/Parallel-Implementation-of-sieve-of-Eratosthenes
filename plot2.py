import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns

mpi_data = pd.read_csv('results_mpi.csv')
sequential_data = pd.read_csv('results_sequential.csv')

mpi_data['log_n'] = np.log10(mpi_data['n'])
sequential_data['log_n'] = np.log10(sequential_data['n'])

num_processes_max = 24

log_n_seq = sequential_data['log_n'].values
exec_time_seq = sequential_data['Execution_Time'].values

log_n_mesh, num_processes_mesh = np.meshgrid(log_n_seq, np.arange(1, num_processes_max + 1))
exec_time_mesh = np.tile(exec_time_seq, (num_processes_max, 1))

sns.set(style="whitegrid")
fig = plt.figure(figsize=(15, 15))
ax = fig.add_subplot(111, projection='3d')

mpi_data_sorted = mpi_data.sort_values(by=['log_n', 'num_processes'])

for log_n_value, group_data in mpi_data_sorted.groupby('log_n'):
    ax.plot(group_data['log_n'], group_data['num_processes'], group_data['Execution_Time'],
            label=f'Parallel, log_n = {log_n_value:.2f}', marker='.', zorder=5)

ax.plot_wireframe(log_n_mesh, num_processes_mesh, exec_time_mesh, color='lightblue', alpha=0.6,
                  rstride=1, cstride=1)

ax.view_init(elev=20, azim=135)
ax.set_xlabel('log(n) (Log Problem Size)')
ax.set_ylabel('Number of Processes')
ax.set_zlabel('Execution Time (s)')
ax.set_title('3D Comparison of Parallel and Sequential Execution Times with Log-Scaled Problem Size with MPI')

ax.legend(loc='upper left')
plt.show()
