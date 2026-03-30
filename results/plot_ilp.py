import pandas as pd
import matplotlib.pyplot as plt

# 1. Load the data
try:
    df = pd.read_csv('pure_ilp_results.csv')
except FileNotFoundError:
    print("Error: 'pure_ilp_results.csv' not found. Run the C++ program first.")
    exit()

# 2. Set up the plot
plt.figure(figsize=(10, 6))

# Plot ILP-Friendly operations
plt.plot(df['L1_Size_KB'], df['Friendly_ns_per_op'], 
         marker='o', linestyle='-', color='blue', label='ILP-Friendly (Independent Additions)')

# Plot ILP-Unfriendly operations
plt.plot(df['L1_Size_KB'], df['Unfriendly_ns_per_op'], 
         marker='s', linestyle='-', color='red', label='ILP-Hostile (Serial Dependency / Fibonacci)')

# 3. Format the graph
plt.xscale('log', base=2)
plt.ylim(bottom=0) # Start Y-axis at 0

# --- THE FIXES ---
# Set explicit limits to bracket our data nicely and avoid the log(0) infinite stretch
plt.xlim(left=0.0625, right=64) 

# Create custom, highly readable tick marks for the x-axis
x_ticks = [0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32]
plt.xticks(x_ticks, labels=[str(x) for x in x_ticks])

plt.xlabel('Working Set Size in L1 Cache (KB) - Log2 Scale', fontsize=12)
plt.ylabel('Time per Math Operation (Nanoseconds)', fontsize=12)
plt.title('Instruction-Level Parallelism within L1 Cache', fontsize=14)

# Start the green span from our new left limit instead of 0
plt.axvspan(0.0625, 48, color='green', alpha=0.1, label='Inside L1 Data Cache Boundaries')
# -----------------

plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend(fontsize=11, loc='center right')

# 4. Save and display
plt.tight_layout()
plt.savefig('pure_ilp_benchmark_fixed.png', dpi=300)
print("Graph saved as 'pure_ilp_benchmark_fixed.png'")
plt.show()