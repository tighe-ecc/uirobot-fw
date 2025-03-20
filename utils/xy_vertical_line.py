#!/usr/bin/env python3

import numpy as np
import os
from animate_path import animate_path
import configparser

# Inputs
distance = 1.0  # Distance to move up in meters
speed = 0.5     # Speed in meters per second
frequency = 40  # Frequency in Hz
joint_id = 6    # Joint ID to include in the logfile

# Calculate the time to traverse the distance
time_to_traverse = distance / speed

# Number of points for the line
num_points = int(time_to_traverse * frequency)

# Generate the coordinates for the vertical line
y = np.linspace(0, distance, num_points)
x = np.zeros(num_points)

# Determine logfile path based on operating system
base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logfile_path = os.path.join(base_path, 'cloudgate', 'setpoints_xy.csv')
config_path = os.path.join(base_path, 'cloudgate', 'config.ini')

# Read the home position from the config.ini file
config = configparser.ConfigParser()
config.read(config_path)
joint_key = f"joint_{joint_id}"
try:
    home_position = config[joint_key]['home']
    home_position = [float(coord) for coord in home_position.split(',')]
except KeyError:
    raise KeyError(f"Joint key '{joint_key}' or 'home' not found in the config file.")

# Offset all y-coordinates by the home position
x += home_position[0]
y += home_position[1]

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