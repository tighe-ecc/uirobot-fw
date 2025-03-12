import numpy as np

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
