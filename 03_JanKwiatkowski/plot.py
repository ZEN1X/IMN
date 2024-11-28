import os
import shutil

import matplotlib.figure
import matplotlib.pyplot as plt
import numpy as np

# general helper funcs


def load_csv(path: str) -> np.ndarray:
    return np.loadtxt(path, delimiter=",")


def ensure_dir_exists(dir: str) -> None:
    if not os.path.exists(dir):
        os.makedirs(dir)


def move_csv(files: list[str]) -> None:
    ensure_dir_exists("output")
    for file in files:
        shutil.move(file, os.path.join("output", file))


def save_plot(fig: matplotlib.figure.Figure, filename: str) -> None:
    ensure_dir_exists("plots")
    fig.savefig(os.path.join("plots", filename))


def clean_build_files(build_dir="build") -> None:
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)


# define files used for plotting
csv_files = ["rk2_2.csv", "rk2_5.csv", "trapeze_2.csv", "trapeze_5.csv"]

# begin actual operation

# load CSVs
data = []
for csv_file in csv_files:
    data.append(load_csv(csv_file))


def plot(data: list[np.ndarray], filenames: list[str]) -> None:
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    axes = axes.flatten()  # flatten for easier looping

    # helper func to set up each
    def setup_axis(ax, title, xlabel, ylabel):
        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        ax.grid(linestyle=':')

    # plot title (same for each of the 4 axes)
    base_title = filenames[0].split("_")[0].upper()
    
    # axis setup: (title, xlabel, ylabel)
    axis_info = [
        (base_title, "Czas (t)", "x(t)"),
        (base_title, "Czas (t)", "v(t)"),
        (base_title, "Czas (t)", "dt(t)"),
        (base_title, "x", "v"),
    ]
    
    # setup each of the axis
    for ax, (title, xlabel, ylabel) in zip(axes, axis_info):
        setup_axis(ax, title, xlabel, ylabel)

    # plot for each of TOLs
    for i, csv_data in enumerate(data):
        label = "TOL = $10^{-2}$" if i == 0 else "TOL = $10^{-5}$"
        t, x_t, v_t, dt = csv_data[:, 0], csv_data[:, 1], csv_data[:, 2], csv_data[:, 3]

        # plot on each of the axis
        axes[0].plot(t, x_t, label=label)
        axes[1].plot(t, v_t, label=label)
        axes[2].plot(t, dt, label=label, marker="o")
        axes[3].plot(x_t, v_t, label=label)

    # add legends (can only do after plotting :/)
    for ax in axes:
        ax.legend(loc="upper right")

    fig.tight_layout()
    save_plot(fig, f"{base_title}.png")

# plot(data, csv_files)
plot(data[:2], csv_files[:2])
plot(data[2:], csv_files[2:])

# organize files
clean_build_files()
move_csv(csv_files)
