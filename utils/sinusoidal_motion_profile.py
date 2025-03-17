import time
import numpy as np

# Input for the period of the ramp up and down (in seconds)
period = 2
amplitude = 6400
duration = 4

# Frequency (Hz)
frequency = 40

# Number of points for each period
num_points = int(period * frequency)

# Generate the sinusoidal motion profile for the entire duration
t = np.linspace(0, duration, num=int(duration * frequency))
profile = amplitude * np.sin(2 * np.pi * t / period)

# Logfile path
logfile_path = 'c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints.csv'

# Collect all setpoints in a list
setpoints = []

for i, setpoint in enumerate(profile):
    timestamp = (i / frequency)
    setpoints.append(f'{timestamp},{setpoint}\n')
    # time.sleep(1 / frequency)  # 40Hz

# Write all setpoints to the logfile at once
with open(logfile_path, 'w') as logfile:
    logfile.writelines(setpoints)
