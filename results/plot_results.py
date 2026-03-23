import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV files
try:
    df_cpp = pd.read_csv('cpp_results.csv')
    df_go = pd.read_csv('go_results.csv')
except FileNotFoundError as e:
    print(f"Error loading files: {e}. Make sure you ran the C++ and Go executables.")
    exit()

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(df_cpp['Size'], df_cpp['Time_ms'], marker='o', label='C++17 (-O3)', color='blue')
plt.plot(df_go['Size'], df_go['Time_ms'], marker='s', label='Go', color='teal')

plt.title('Matrix Multiplication Performance: C++17 vs Go')
plt.xlabel('Matrix Size (N x N)')
plt.ylabel('Execution Time (milliseconds)')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)

# Save the graphic and display it
plt.savefig('performance_comparison.png')
plt.show()