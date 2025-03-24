import matplotlib.pyplot as plt
import numpy as np

# Dataset names
datasets = ["Wiki-Vote", "Email-Enron", "AS-Skitter"]

# Execution times in seconds
execution_times_sec = [3.469, 1.92, 1141]

# Convert execution times to minutes
execution_times_min = [t / 60 for t in execution_times_sec]

# Plot histogram
plt.figure(figsize=(8, 5))
bars = plt.bar(datasets, execution_times_min, color=['blue', 'green', 'red'])

# Annotate bars with exact values
for bar, time in zip(bars, execution_times_min):
    plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 1, f"{time:.2f} min",
             ha='center', fontsize=12, fontweight='bold')

# Labels and title
plt.ylabel("Execution Time (minutes)", fontsize=12)
plt.xlabel("Datasets", fontsize=12)
plt.title("Execution Time of ELS Algorithm", fontsize=14)
plt.yscale("log")  # Log scale to handle skewed distribution
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Show plot
plt.show()
