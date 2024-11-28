import os
import shutil

import matplotlib.figure
import matplotlib.pyplot as plt
import numpy as np

# general helper funcs


def load_csv(path: str) -> None:
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
csv_files = ["picard.csv", "newton.csv", "rk2.csv"]
mu_files = ["picard_mu.csv", "newton_mu.csv", "rk2_mu.csv"]

# begin actual operation

# load CSVs
data = []
for csv_file in csv_files:
    data.append(load_csv(csv_file))

data_mu = []
for mu_file in mu_files:
    data_mu.append(load_csv(mu_file))


def plot(data: list[np.ndarray], filenames: list[str]) -> None:
    N = 500  # z(t) = N - u(t)
    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)

    for i, (csv_data, filename) in enumerate(zip(data, filenames)):
        t = csv_data[:, 0]  # time
        u_t = csv_data[:, 1]  # u(t)
        z_t = N - u_t  # z(t) = N - u(t)

        axes[i].plot(t, u_t, label="u(t)")
        axes[i].plot(t, z_t, label="z(t) = N - u(t)")

        # limit
        axes[i].set_xlim(0, 100)

        # labels
        title = filename.replace('.csv', '').upper()
        axes[i].set_title(title)
        axes[i].set_xlabel("Czas (t)")
        axes[i].set_ylabel("u(t), z(t)")
        axes[i].legend()
        axes[i].grid(linestyle=':')

    fig.tight_layout()
    save_plot(fig, "plot.png")

def plot_mu(data: list[np.ndarray], filenames: list[str]) -> None:
    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)

    for i, (csv_data, filename) in enumerate(zip(data, filenames)):
        t = csv_data[:, 0]  # time
        n_t = csv_data[:, 1]  # n(t)

        axes[i].plot(t, n_t, label="n(t)")

        # limit
        axes[i].set_xlim(0, 100)

        # labels
        title = filename.replace('.csv', '').upper()
        axes[i].set_title(title)
        axes[i].set_xlabel("Czas (t)")
        axes[i].set_ylabel("n(t) (iteracje)")
        axes[i].legend()
        axes[i].grid(linestyle=':')

    fig.tight_layout()
    save_plot(fig, "plot_mu.png")

    


plot(data, csv_files)
plot_mu(data_mu, mu_files)


# organize files
clean_build_files()
move_csv(csv_files)
move_csv(mu_files)
