import pandas as pd
import matplotlib.pyplot as plt
import sys

# Helper function to load files and handle the UTF-16 PowerShell quirk automatically
def load_data(filename):
    try:
        return pd.read_csv(filename)
    except UnicodeDecodeError:
        return pd.read_csv(filename, encoding='utf-16')
    except FileNotFoundError:
        print(f"Error: Could not find '{filename}'. Make sure all 4 CSV files are in this folder.")
        sys.exit()

# 1. Load all four datasets
df_cpp_classic = load_data('cpp_results.csv')
df_go_classic = load_data('go_results.csv')
df_cpp_blocks = load_data('blocks_results.csv')
df_go_blocks = load_data('go_blocks_results.csv')

# 2. Convert block times from nanoseconds to milliseconds to match the classic data
df_cpp_blocks['Time_ms'] = df_cpp_blocks['Time_ns'] / 1_000_000
df_go_blocks['Time_ms'] = df_go_blocks['Time_ns'] / 1_000_000

# Get the unique block sizes to loop through
block_sizes = df_cpp_blocks['BlockSize'].unique()

# Create a figure with two side-by-side subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6), sharey=True)

# ----------------- Plot C++ Results -----------------
# Plot the Classic baseline as a thick, dashed black line
ax1.plot(df_cpp_classic['Size'], df_cpp_classic['Time_ms'], 
         color='black', linewidth=3, linestyle='--', marker='X', label='Classic (Naive)')

# Plot the Block sizes normally
for bs in block_sizes:
    data = df_cpp_blocks[df_cpp_blocks['BlockSize'] == bs]
    ax1.plot(data['MatrixSize'], data['Time_ms'], marker='o', alpha=0.8, label=f'Block Size {bs}')

ax1.set_title('C++17: Classic vs Cache-Optimized Blocks')
ax1.set_xlabel('Matrix Size (N x N)')
ax1.set_ylabel('Execution Time (milliseconds)')
ax1.legend()
ax1.grid(True, linestyle='--', alpha=0.7)

# ----------------- Plot Go Results -----------------
# Plot the Classic baseline as a thick, dashed black line
ax2.plot(df_go_classic['Size'], df_go_classic['Time_ms'], 
         color='black', linewidth=3, linestyle='--', marker='X', label='Classic (Naive)')

for bs in block_sizes:
    data = df_go_blocks[df_go_blocks['BlockSize'] == bs]
    ax2.plot(data['MatrixSize'], data['Time_ms'], marker='s', alpha=0.8, label=f'Block Size {bs}')

ax2.set_title('Go: Classic vs Cache-Optimized Blocks')
ax2.set_xlabel('Matrix Size (N x N)')
ax2.legend()
ax2.grid(True, linestyle='--', alpha=0.7)

# Adjust layout, save, and display
plt.tight_layout()
plt.savefig('ultimate_comparison.png')
plt.show()