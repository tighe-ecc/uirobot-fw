import numpy as np
from animate_path import animate_path

# Inputs
distance = 0.50  # Distance to move up in meters
speed = 0.05     # Speed in meters per second
frequency = 40  # Frequency in Hz

# Calculate the time to traverse the distance
time_to_traverse = distance / speed

# Number of points for the line
num_points = int(time_to_traverse * frequency)

# Generate the coordinates for the vertical line
y = np.linspace(0, distance, num_points)
x = np.zeros(num_points)

# Logfile path
logfile_path = 'c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints_xy.csv'

# Collect all setpoints in a list
setpoints = []

for i in range(len(x)):
    timestamp = (i / frequency)
    setpoints.append(f'{timestamp},{x[i]},{y[i]}\n')

# Write all setpoints to the logfile at once
with open(logfile_path, 'w') as logfile:
    logfile.writelines(setpoints)

animate_path(x, y, frequency)