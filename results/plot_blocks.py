import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV files
try:
    # Tries standard UTF-8 first (if you used Out-File)
    df_cpp = pd.read_csv('blocks_results.csv')
    df_go = pd.read_csv('go_blocks_results.csv')
except UnicodeDecodeError:
    # Falls back to UTF-16 just in case you used the standard > operator in PowerShell
    df_cpp = pd.read_csv('blocks_results.csv', encoding='utf-16')
    df_go = pd.read_csv('go_blocks_results.csv', encoding='utf-16')
except FileNotFoundError as e:
    print(f"Error loading files: {e}. Make sure you ran the C++ and Go executables.")
    exit()

# Convert nanoseconds to milliseconds for easier readability
df_cpp['Time_ms'] = df_cpp['Time_ns'] / 1_000_000
df_go['Time_ms'] = df_go['Time_ns'] / 1_000_000

# Get the unique block sizes
block_sizes = df_cpp['BlockSize'].unique()

# Create a figure with two side-by-side subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6), sharey=True)

# ----------------- Plot C++ Results -----------------
for bs in block_sizes:
    data = df_cpp[df_cpp['BlockSize'] == bs]
    ax1.plot(data['MatrixSize'], data['Time_ms'], marker='o', label=f'Block Size {bs}')

ax1.set_title('C++17 (-O3) Block Matrix Multiplication')
ax1.set_xlabel('Matrix Size (N x N)')
ax1.set_ylabel('Execution Time (milliseconds)')
ax1.legend()
ax1.grid(True, linestyle='--', alpha=0.7)

# ----------------- Plot Go Results -----------------
for bs in block_sizes:
    data = df_go[df_go['BlockSize'] == bs]
    ax2.plot(data['MatrixSize'], data['Time_ms'], marker='s', label=f'Block Size {bs}')

ax2.set_title('Go Block Matrix Multiplication')
ax2.set_xlabel('Matrix Size (N x N)')
ax2.legend()
ax2.grid(True, linestyle='--', alpha=0.7)

# Adjust layout, save, and display
plt.tight_layout()
plt.savefig('block_performance_comparison.png')
plt.show()