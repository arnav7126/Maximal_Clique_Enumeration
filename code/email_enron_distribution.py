import matplotlib.pyplot as plt

# Clique size distribution data for Email-Enron dataset
clique_sizes = list(range(2, 21))  # Clique sizes from 2 to 20
clique_counts = [14070, 7077, 13319, 18143, 22715, 25896, 24766, 22884, 21393, 
                 17833, 15181, 11487, 7417, 3157, 1178, 286, 41, 10, 6]  # Number of maximal cliques

# Plot histogram
plt.figure(figsize=(10, 6))
bars = plt.bar(clique_sizes, clique_counts, color='purple', alpha=0.7)

# Annotate bars with exact values
for bar, count in zip(bars, clique_counts):
    if count > 0:  # Show only significant values to avoid clutter
        plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 200, f"{count}",
                 ha='center', fontsize=10, fontweight='bold')

# Labels and title
plt.xlabel("Clique Size", fontsize=12)
plt.ylabel("Number of Maximal Cliques", fontsize=12)
plt.title("Clique Size Distribution in Email-Enron Dataset", fontsize=14)
plt.xticks(clique_sizes)  # Show all clique sizes on x-axis
plt.grid(axis='y', linestyle='--', alpha=0.6)

# Show plot
plt.show()
