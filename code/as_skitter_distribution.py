import matplotlib.pyplot as plt

# Clique size distribution data for AS-Skitter dataset
clique_sizes_skitter = list(range(2, 68))  # Clique sizes from 2 to 67
clique_counts_skitter = [
    2319807, 3171609, 1823321, 939336, 684873, 598284, 588889, 608937, 665661, 728098, 
    798073, 877282, 945194, 980831, 939987, 839330, 729601, 639413, 600192, 611976, 
    640890, 673924, 706753, 753633, 818353, 892719, 955212, 999860, 1034106, 1055653, 
    1017560, 946717, 878552, 809485, 744634, 663650, 583922, 520239, 474301, 420796, 
    367879, 321829, 275995, 222461, 158352, 99522, 62437, 39822, 30011, 25637, 17707, 
    9514, 3737, 2042, 1080, 546, 449, 447, 405, 283, 242, 146, 84, 49, 22, 4
]

# Plot histogram
plt.figure(figsize=(12, 6))
bars = plt.bar(clique_sizes_skitter, clique_counts_skitter, color='darkred', alpha=0.7)

# Annotate bars with exact values for significant counts
for bar, count in zip(bars, clique_counts_skitter):
    if count > 3000000 or count < 5:  # Show for large and very small values
        plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 50000, f"{count}",
                 ha='center', fontsize=8, fontweight='bold')

# Labels and title
plt.xlabel("Clique Size", fontsize=12)
plt.ylabel("Number of Maximal Cliques", fontsize=12)
plt.title("Clique Size Distribution in AS-Skitter Dataset", fontsize=14)
plt.xticks(range(2, 68, 2))  # Show every 2nd clique size for readability
plt.yscale("log")  # Use log scale to handle large range
plt.grid(axis='y', linestyle='--', alpha=0.6)

# Show plot
plt.show()
