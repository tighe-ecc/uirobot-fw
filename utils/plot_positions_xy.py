import pandas as pd
import matplotlib.pyplot as plt
import glob

# Read the CSV files
df_setpoint = pd.read_csv('utils/setpoint_log.csv')
setpoint_time = df_setpoint.iloc[:, 0]
setpoint_position = df_setpoint.iloc[:, 1:]

# Find all position_log* files
position_files = glob.glob('utils/position_log*.csv')

# Create subplots
fig, axs = plt.subplots(len(position_files), 1, figsize=(10, 6 * len(position_files)))

# Reverse the file list
# position_files = position_files[::-1]

for i, file in enumerate(position_files):
    df_position = pd.read_csv(file)
    
    # Extract data from logs
    clock_time = df_position['ClockTime'] - df_position['ClockTime'].iloc[0]  # Start at t=0
    absolute_position = df_position['ActualPosition']
    setpoint_velocity = df_position['SetpointVelocity']

    # Calculate the time deltas
    time_deltas = clock_time.diff().dropna()
    max_time_delta = time_deltas.max()
    min_time_delta = time_deltas.min()

    ax1 = axs[i] if len(position_files) > 1 else axs
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Position')
    ax1.plot(setpoint_time, setpoint_position.iloc[:,i], label='Setpoint Position', color='tab:blue', marker='o')
    ax1.plot(clock_time, absolute_position, label='Actual Position', color='tab:orange', marker='o')
    ax1.tick_params(axis='y')
    ax1.legend(loc='upper left')
    ax1.grid(True)

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
    ax2.set_ylabel('Velocity')  # we already handled the x-label with ax1
    ax2.plot(clock_time, setpoint_velocity, label='Setpoint Velocity', color='tab:green', marker='o')
    ax2.tick_params(axis='y')
    ax2.legend(loc='upper right')

    # Add max and min time delta to the title
    ax1.set_title(f'{file}\nMax dt: {int(max_time_delta * 1000)}ms, Min dt: {int(min_time_delta * 1000)}ms')

fig.tight_layout()  # otherwise the right y-label is slightly clipped

# Set the position of the plot window to the top left of the screen
manager = plt.get_current_fig_manager()
manager.window.wm_geometry("+0+0")

plt.show()
