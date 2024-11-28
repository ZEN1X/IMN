import os
import shutil
import matplotlib.figure
import matplotlib.pyplot as plt
import numpy as np


# helper funcs
def load_csv(path: str) -> np.ndarray:
    return np.loadtxt(path, delimiter=",", dtype=float)


def ensure_dir_exists(directory: str) -> None:
    if not os.path.exists(directory):
        os.makedirs(directory)


def save_plot(fig: matplotlib.figure.Figure, filename: str) -> None:
    ensure_dir_exists("plots")
    fig.savefig(os.path.join("plots", filename))


def move_csv(files: list[str]) -> None:
    ensure_dir_exists("output")
    for file in files:
        shutil.move(file, os.path.join("output", file))


def clean_build_files(build_dir="build") -> None:
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)


# files
global_files = {"S06": "S06_iter.csv", "S10": "S10_iter.csv"}

local_files = {
    "local10": "local10_iter.csv",
    "local14": "local14_iter.csv",
    "local18": "local18_iter.csv",
    "local19": "local19_iter.csv",
}

solution_files = {"S06": "S06_sol.csv", "S10": "S10_sol.csv"}

error_files = {"S06": "S06_err.csv", "S10": "S10_err.csv"}


# helper func to extract 'w' from filenames
def extract_w_from_filename(filename: str, prefix: str) -> float:
    return float(filename.split(prefix)[1].split("_")[0]) / 10


# load the data
global_data = {key: load_csv(path) for key, path in global_files.items()}
local_data = {key: load_csv(path) for key, path in local_files.items()}
solution_data = {key: load_csv(path) for key, path in solution_files.items()}
error_data = {key: load_csv(path) for key, path in error_files.items()}

# calculate no of iterations and extract "w" from filenames
global_iterations = {key: len(values) for key, values in global_data.items()}
local_iterations = {key: len(values) for key, values in local_data.items()}
global_w_values = {
    key: extract_w_from_filename(key, "S") for key in global_files.keys()
}
local_w_values = {
    key: extract_w_from_filename(key, "local") for key in local_files.keys()
}
solution_w_values = {
    key: extract_w_from_filename(key, "S") for key in solution_files.keys()
}


# functions for plotting
def plot_iter(
    global_data,
    local_data,
    global_w_values,
    local_w_values,
    global_iterations,
    local_iterations,
):
    fig, axes = plt.subplots(1, 2, figsize=(14, 7))

    # global relax
    ax_global = axes[0]
    ax_global.set_title("Relaksacja globalna")
    ax_global.set_xlabel("Iteracja")
    ax_global.set_ylabel("S")
    ax_global.set_xscale("log")
    ax_global.grid(True, linestyle=":")

    for key, values in global_data.items():
        w = global_w_values[key]
        iter_count = global_iterations[key]
        label = f"w={w}, iter={iter_count-2}"
        ax_global.plot(range(3, iter_count + 1), values[2:], label=label)

    ax_global.legend(loc="upper right")

    # local relax
    ax_local = axes[1]
    ax_local.set_title("Relaksacja lokalna")
    ax_local.set_xlabel("Iteracja")
    ax_local.set_ylabel("S")
    ax_local.set_xscale("log")
    ax_local.grid(True, linestyle=":")

    for key, values in local_data.items():
        w = local_w_values[key]
        iter_count = local_iterations[key]
        label = f"w={w}, iter={iter_count-2}"
        ax_local.plot(range(3, iter_count + 1), values[2:], label=label)

    ax_local.legend(loc="upper right")

    fig.tight_layout()
    save_plot(fig, "iter.png")


def plot_heatmaps(
    data,
    w_values,
    x_scale=10,
    y_scale=10,
    cmap="RdBu_r",
    title_prefix="Relaksacja",
    output_filename="heatmap.png",
):
    fig, axes = plt.subplots(1, 2, figsize=(14, 7))

    for ax, (key, values) in zip(axes, data.items()):
        w = w_values[key]
        heatmap_data = values
        ax.set_title(f"{title_prefix} w={w:.1f}")
        heatmap = ax.imshow(
            heatmap_data,
            aspect="auto",
            origin="lower",
            cmap=cmap,
            vmin=np.min(heatmap_data),
            vmax=np.max(heatmap_data),
        )

        # scale ticks by 10
        x_ticks = np.linspace(0, heatmap_data.shape[1], 15)
        y_ticks = np.linspace(0, heatmap_data.shape[0], 10)
        ax.set_xticks(x_ticks)
        ax.set_xticklabels((x_ticks / x_scale).astype(int))
        ax.set_yticks(y_ticks)
        ax.set_yticklabels((y_ticks / y_scale).astype(int))

        ax.set_xlabel("x")
        ax.set_ylabel("y")
        fig.colorbar(heatmap, ax=ax, orientation="vertical")

    fig.tight_layout()
    save_plot(fig, output_filename)


# plot
plot_iter(
    global_data,
    local_data,
    global_w_values,
    local_w_values,
    global_iterations,
    local_iterations,
)
plot_heatmaps(
    solution_data,
    solution_w_values,
    x_scale=10,
    y_scale=10,
    cmap="RdBu_r",
    title_prefix="Relaksacja globalna",
    output_filename="solution.png",
)
plot_heatmaps(
    error_data,
    solution_w_values,
    x_scale=10,
    y_scale=10,
    cmap="hot",
    title_prefix="Błąd relaksacja globalna",
    output_filename="error.png",
)

# move CSVs
all_files = (
    list(global_files.values())
    + list(local_files.values())
    + list(solution_files.values())
    + list(error_files.values())
)
# move_csv(all_files)

# clean build dir
# clean_build_files()
