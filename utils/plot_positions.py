import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV files
df_setpoint = pd.read_csv('cloudgate/setpoints.csv')
df_position = pd.read_csv('utils/position_log.csv')

# Extract the time, setpoint position, absolute position, and setpoint velocity columns
setpoint_time = df_setpoint.iloc[:, 0]
setpoint_position = df_setpoint.iloc[:, 1]
clock_time = df_position['ClockTime']
absolute_position = df_position['ActualPosition']
setpoint_velocity = df_position['SetpointVelocity']

# Calculate the time deltas
time_deltas = clock_time.diff().dropna()
max_time_delta = time_deltas.max()
min_time_delta = time_deltas.min()

# Plot the data
fig, ax1 = plt.subplots(figsize=(10, 6))

ax1.set_xlabel('Time')
ax1.set_ylabel('Position')
ax1.plot(setpoint_time, setpoint_position, label='Setpoint Position', color='tab:blue', marker='o')
ax1.plot(clock_time, absolute_position, label='Absolute Position', color='tab:orange', marker='o')
ax1.tick_params(axis='y')
ax1.legend(loc='upper left')
ax1.grid(True)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2.set_ylabel('Velocity')  # we already handled the x-label with ax1
ax2.plot(clock_time, setpoint_velocity, label='Setpoint Velocity', color='tab:green', marker='o')
ax2.tick_params(axis='y')
ax2.legend(loc='upper right')

# Add max and min time delta to the title
plt.title(f'Setpoint Position vs Absolute Position and Setpoint Velocity\nMax dt: {int(max_time_delta * 1000)}ms, Min dt: {int(min_time_delta * 1000)}ms')
fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()
