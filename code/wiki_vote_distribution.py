import matplotlib.pyplot as plt

# Clique size distribution data for Wiki-Vote dataset
clique_sizes_wiki_vote = list(range(2, 18))  # Clique sizes from 2 to 17
clique_counts_wiki_vote = [8655, 13718, 27292, 48416, 68872, 83266, 76732, 54456, 35470, 
                           21736, 11640, 5449, 2329, 740, 208, 23]  # Number of maximal cliques

# Plot histogram
plt.figure(figsize=(10, 6))
bars = plt.bar(clique_sizes_wiki_vote, clique_counts_wiki_vote, color='blue', alpha=0.7)

# Annotate bars with exact values
for bar, count in zip(bars, clique_counts_wiki_vote):
    if count > 0:  # Show only significant values to avoid clutter
        plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 500, f"{count}",
                 ha='center', fontsize=10, fontweight='bold')

# Labels and title
plt.xlabel("Clique Size", fontsize=12)
plt.ylabel("Number of Maximal Cliques", fontsize=12)
plt.title("Clique Size Distribution in Wiki-Vote Dataset", fontsize=14)
plt.xticks(clique_sizes_wiki_vote)  # Show all clique sizes on x-axis
plt.grid(axis='y', linestyle='--', alpha=0.6)

# Show plot
plt.show()
