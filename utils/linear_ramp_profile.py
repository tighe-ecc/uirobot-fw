import time
import numpy as np

# Input for the period of the ramp up and down (in seconds)
period = 1

# Frequency (Hz)
frequency = 10

# Number of points for each ramp
num_points = int(period * frequency / 2)

# Generate linear ramp profile
ramp_up = np.linspace(0, 3200, num=num_points)
ramp_down = np.linspace(3200, 0, num=num_points)
profile = np.concatenate((ramp_up, ramp_down))

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
