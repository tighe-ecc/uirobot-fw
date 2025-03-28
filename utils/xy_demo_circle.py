#!/usr/bin/env python3

import numpy as np
import os
from animate_path import animate_path

# Inputs
diameter = 0.5  # Diameter of the circle in meters
speed = 0.1     # Speed in meters per second
frequency = 40  # Frequency in Hz

# Calculate the circumference of the circle
circumference = np.pi * diameter

# Calculate the time to traverse the circle
time_to_traverse = circumference / speed

# Number of points for the circle
num_points = int(time_to_traverse * frequency)

# Generate the coordinates for the circle
theta = np.linspace(0, 2 * np.pi, num_points)
x = (diameter / 2) * np.cos(theta) - (diameter / 2)
y = (diameter / 2) * np.sin(theta)

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