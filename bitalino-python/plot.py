import matplotlib.pyplot as plt
import numpy as np

def plot_emg(emg_data):
    plt.figure()
    plt.plot(emg_data)
    plt.title("EMG Data")
    plt.xlabel("Time")
    plt.ylabel("EMG Value")
    plt.show()

def plot_emg_realtime(emg_data, speed_data):
    plt.ion()
    fig, (ax1, ax2) = plt.subplots(2, 1)
    line1, = ax1.plot(emg_data)
    line2, = ax2.plot(speed_data)
    ax1.set_title("Real-time EMG Data")
    ax1.set_xlabel("Time")
    ax1.set_ylabel("EMG Value")
    ax2.set_title("Real-time Speed Data")
    ax2.set_xlabel("Time")
    ax2.set_ylabel("Speed Value")
    return fig, ax1, line1, ax2, line2

def update_plot(fig, ax1, line1, emg_data, ax2, line2, speed_data):
    line1.set_ydata(emg_data)
    line1.set_xdata(np.arange(len(emg_data)))
    ax1.relim()
    ax1.autoscale_view()
    
    line2.set_ydata(speed_data)
    line2.set_xdata(np.arange(len(speed_data)))
    ax2.relim()
    ax2.autoscale_view()
    
    fig.canvas.draw()
    fig.canvas.flush_events()
