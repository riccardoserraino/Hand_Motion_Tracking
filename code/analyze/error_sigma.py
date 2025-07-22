from config import *

# Example lists (replace with your actual data)
error_values = [0.28825347, 0.2227711, 0.2460858686, 0.28314236, 0.484595724]
error_values1 = [0.1896029057, 0.1733234237, 0.214462600, 0.1845543, 0.245916965]
error_values2 = [0.3577424, 0.29184209, 0.318948569, 0.213921375, 0.2226940445]

error_names = ['power_grasp', 'pinch_grasp', 'ulnar_grasp', 'middle_grasp', 'thumb_up']
error_names1 = ['thumb_flexion', 'index_flexion', 'middle_flexion', 'ring_flexion', 'pinky_flexion']
error_names2 = ['tablet_grasp', 'bottle_grasp', 'tongs_grasp', 'phone_grasp', 'pen_grasp']

# Create the bar plot
plt.figure(figsize=(8, 5)) # Adjust figure size as needed
bars = plt.bar(error_names2, error_values2, color='darkred', width=0.3)

# Add labels and title
#plt.xlabel('Gesture')
plt.ylabel('Error')

# Rotate x-axis labels for better readability if names are long
plt.xticks(rotation=0, ha='center')

# Add grid for better readability (optional)
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Add values on top of each bar
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 0.005, round(yval, 3), ha='center', va='bottom')

plt.ylim(0, 0.525)
# Ensure layout is tight to prevent labels from being cut off
plt.tight_layout()

# Save the plot to a file
plt.show()

print("Bar plot 'error_bar_plot.png' created successfully!")