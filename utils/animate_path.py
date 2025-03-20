import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Button

def animate_path(x, y, frequency):
    # Calculate the extents of the plot
    x_min, x_max = -2.97, 2.97
    y_min, y_max = 0, 4.72
    x_range = x_max - x_min
    y_range = y_max - y_min
    x_margin = 0.05 * x_range
    y_margin = 0.05 * y_range

    # Animate the path
    fig, ax = plt.subplots()
    ax.set_xlim(x_min - x_margin, x_max + x_margin)
    ax.set_ylim(y_min - y_margin, y_max + y_margin)
    ax.grid(True)  # Add gridlines
    ax.set_aspect('equal')  # Make the grid square

    # Add a red box to show the x_range and y_range
    rect = plt.Rectangle((x_min, y_min), x_range, y_range, 
                         linewidth=2, edgecolor='red', facecolor='none')
    ax.add_patch(rect)

    # Add a green box to show the screen
    rect = plt.Rectangle((-2.96, 0), 5.92, 3.05, 
                         linewidth=2, edgecolor='green', facecolor='none')
    ax.add_patch(rect)

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