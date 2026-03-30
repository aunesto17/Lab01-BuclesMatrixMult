import pandas as pd
import matplotlib.pyplot as plt

# 1. Load the data
try:
    df = pd.read_csv('tlb_results.csv')
except FileNotFoundError:
    print("Error: 'tlb_results.csv' not found. Please run the C++ program first.")
    exit()

# 2. Set up the plot
plt.figure(figsize=(10, 6))

# Plot TLB-Friendly (Sequential)
plt.plot(df['PagesAccessed'], df['TLBFriendly_ns'], 
         marker='o', linestyle='-', color='blue', label='TLB-Friendly (Sequential Access)')

# Plot TLB-Unfriendly (Page Striding)
plt.plot(df['PagesAccessed'], df['TLBUnfriendly_ns'], 
         marker='s', linestyle='--', color='red', label='TLB-Unfriendly (Page Striding)')

# 3. Format the graph
plt.xscale('log', base=2)
plt.yscale('log', base=10) # Log scale helps see the massive latency penalty
plt.xlabel('Number of Unique Memory Pages Accessed (Log2 Scale)', fontsize=12)
plt.ylabel('Time per Integer Read (Nanoseconds) - Log10 Scale', fontsize=12)
plt.title('Virtual Memory & TLB Miss Penalty', fontsize=14)

# 4. Add Typical TLB Hardware Boundaries
# L1 DTLB is often small (e.g., 64 entries)
plt.axvline(x=64, color='grey', linestyle=':', alpha=0.8, label='~Typical L1 TLB Limit (64 pages)')

# The text mentions a typical TLB might hold 16 to 512 entries
plt.axvline(x=512, color='orange', linestyle='-.', alpha=0.8, label='~Textbook TLB Limit (512 pages)')

# Modern CPUs (like your Raptor Lake) often have a larger L2 TLB (e.g., 2048 entries)
plt.axvline(x=2048, color='purple', linestyle='--', alpha=0.8, label='~Modern L2 TLB Limit (2048 pages)')

plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend(fontsize=10, loc='upper left')

# 5. Save and display
plt.tight_layout()
plt.savefig('tlb_benchmark_results.png', dpi=300)
print("Graph saved as 'tlb_benchmark_results.png'")
plt.show()