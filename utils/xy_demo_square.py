#!/usr/bin/env python3

import numpy as np
import os
from animate_path import animate_path

# Inputs
height = 0.5  # Height of the rectangle in meters
width = 0.5   # Width of the rectangle in meters
speed = 0.1   # Speed in meters per second
frequency = 40  # Frequency in Hz

# Calculate the time to traverse each side of the rectangle
time_per_side = max(height, width) / speed

# Number of points for each side
num_points_per_side = int(time_per_side * frequency)

# Generate the coordinates for each side of the rectangle
x1 = np.linspace(0, width, num_points_per_side)
y1 = np.zeros(num_points_per_side)

x2 = np.full(num_points_per_side, width)
y2 = np.linspace(0, height, num_points_per_side)

x3 = np.linspace(width, 0, num_points_per_side)
y3 = np.full(num_points_per_side, height)

x4 = np.zeros(num_points_per_side)
y4 = np.linspace(height, 0, num_points_per_side)

# Concatenate the coordinates to form the complete rectangle path
x = np.concatenate([x1, x2, x3, x4])
y = np.concatenate([y1, y2, y3, y4])

# Determine logfile path based on operating system
base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logfile_path = os.path.join(base_path, 'cloudgate', 'setpoints_xy.csv')

# Collect all setpoints in a list
setpoints = []

for i in range(len(x)):
    timestamp = (i / frequency)
    setpoints.append(f'{timestamp},{x[i]},{y[i]}\n')

# Write all setpoints to the logfile at once
try:
    with open(logfile_path, 'w') as logfile:
        logfile.writelines(setpoints)
    print(f"Successfully wrote setpoints to {logfile_path}")
except Exception as e:
    print(f"Error writing to logfile: {e}")
    
# Call the animate_path function
animate_path(x, y, frequency)