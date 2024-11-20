import pandas as pd
import matplotlib.pyplot as plt

# Load the results
df = pd.read_csv("/content/results.txt")

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(df['n'], df['Sequential_Time'], label='Sequential', marker='o')
plt.plot(df['n'], df['OpenMP_Time'], label='OpenMP', marker='o')
plt.plot(df['n'], df['MPI_Time'], label='MPI', marker='o')

plt.xscale('log')
plt.yscale('log')
plt.xlabel('n')
plt.ylabel('Time (seconds)')
plt.title('Execution Time of Sieve of Eratosthenes')
plt.legend()
plt.grid(True, which="both", ls="--")

plt.savefig("execution_time_plot.png")
plt.show()
