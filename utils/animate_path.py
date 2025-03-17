import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Button

def animate_path(x, y, frequency):
    # Calculate the extents of the plot
    x_min, x_max = np.min(x), np.max(x)
    y_min, y_max = np.min(y), np.max(y)
    x_range = x_max - x_min
    y_range = y_max - y_min
    x_margin = 0.2 * x_range
    y_margin = 0.2 * y_range

    # Animate the path
    fig, ax = plt.subplots()
    ax.set_xlim(x_min - x_margin, x_max + x_margin)
    ax.set_ylim(y_min - y_margin, y_max + y_margin)
    ax.grid(True)  # Add gridlines
    ax.set_aspect('equal')  # Make the grid square

    line, = ax.plot([], [], 'b-')
    dot, = ax.plot([], [], 'bo')  # Add a red dot

    def init():
        line.set_data([], [])
        dot.set_data([], [])
        return line, dot

    skip_factor = 1

    def update(frame):
        frame = frame * skip_factor
        if frame >= len(x):
            frame = len(x) - 1
        line.set_data(x[:frame], y[:frame])
        dot.set_data([x[frame]], [y[frame]])  # Update the dot position
        return line, dot

    def increase_speed(event):
        nonlocal skip_factor
        skip_factor *= 2
        ani.event_source.interval = 1000 / (frequency * skip_factor)

    ani = FuncAnimation(fig, update, frames=range(len(x) // skip_factor), init_func=init, blit=True, interval=1000/frequency)

    # Add a button to increase speed by skipping points
    ax_button = plt.axes([0.8, 0.01, 0.1, 0.05])
    button = Button(ax_button, 'x2')
    button.on_clicked(increase_speed)

    plt.show()