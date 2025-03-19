#!/usr/bin/env python3

import numpy as np
import os
from animate_path import animate_path

# Inputs
height = 2.0  # Height of the plus shape in meters
width = 1.0   # Width of the plus shape in meters
speed = 1.0  # Speed in meters per second
frequency = 40  # Frequency in Hz
joint_id = 3    # Joint ID to include in the logfile

# Calculate the time to traverse the height and width
time_to_traverse_height = height / speed
time_to_traverse_width = width / speed

# Number of points for the vertical and horizontal lines
num_points_height = int(time_to_traverse_height * frequency)
num_points_width = int(time_to_traverse_width * frequency)

# Generate the coordinates for the plus shape
y_vertical_up = np.linspace(0, height / 2, num_points_height)
x_vertical_up = np.zeros(num_points_height)

y_horizontal_left = np.ones(num_points_width) * (height / 2)
x_horizontal_left = np.linspace(0, -width / 2, num_points_width)

y_horizontal_right = np.ones(num_points_width) * (height / 2)
x_horizontal_right = np.linspace(0, width / 2, num_points_width)

y_vertical_down = np.linspace(height / 2, height, num_points_height)
x_vertical_down = np.zeros(num_points_height)

# Combine the coordinates to form the plus shape
x = np.concatenate([x_vertical_up, x_horizontal_left, x_horizontal_left[::-1], x_horizontal_right, x_horizontal_right[::-1], x_vertical_down, x_vertical_down[::-1]])
y = np.concatenate([y_vertical_up, y_horizontal_left, y_horizontal_left[::-1], y_horizontal_right, y_horizontal_right[::-1], y_vertical_down, y_vertical_down[::-1]])

# Determine logfile path based on operating system
base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logfile_path = os.path.join(base_path, 'cloudgate', 'setpoints_xy.csv')

# Collect all setpoints in a list
setpoints = []

for i in range(len(x)):
    timestamp = (i / frequency)
    setpoints.append(f'{timestamp},{joint_id},{x[i]},{y[i]}\n')

# Write all setpoints to the logfile at once
try:
    with open(logfile_path, 'w') as logfile:
        logfile.writelines(setpoints)
    print(f"Successfully wrote setpoints to {logfile_path}")
except Exception as e:
    print(f"Error writing to logfile: {e}")
    
# Call the animate_path function
animate_path(x, y, frequency)
