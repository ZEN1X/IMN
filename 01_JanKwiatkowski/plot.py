import os
import shutil
import numpy as np
import matplotlib.pyplot as plt

def load_csv(file_path):
    return np.loadtxt(file_path, delimiter=',')

def ensure_directory_exists(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def move_csv_to_output(file_paths):
    ensure_directory_exists("output")
    for file_path in file_paths:
        shutil.move(file_path, os.path.join("output", file_path))

def save_plot(fig, filename):
    ensure_directory_exists("plots")
    fig.savefig(os.path.join("plots", filename))

# organize files
csv_files = [
    'analytic_0.01.csv', 'analytic_0.10.csv', 'analytic_1.00.csv',
    'euler_0.01.csv', 'euler_0.10.csv', 'euler_1.00.csv',
    'rk2_0.01.csv', 'rk2_0.10.csv', 'rk2_1.00.csv',
    'rk4_0.01.csv', 'rk4_0.10.csv', 'rk4_1.00.csv',
    'rlc_50.00.csv', 'rlc_80.00.csv', 'rlc_100.00.csv', 'rlc_120.00.csv'
]

move_csv_to_output(csv_files)

# delete the build subdir
def delete_build_directory():
    build_dir = "build"
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

delete_build_directory()

# plotting
analytic_001 = load_csv('output/analytic_0.01.csv')
analytic_010 = load_csv('output/analytic_0.10.csv')
analytic_100 = load_csv('output/analytic_1.00.csv')

euler_001 = load_csv('output/euler_0.01.csv')
euler_010 = load_csv('output/euler_0.10.csv')
euler_100 = load_csv('output/euler_1.00.csv')

rk2_001 = load_csv('output/rk2_0.01.csv')
rk2_010 = load_csv('output/rk2_0.10.csv')
rk2_100 = load_csv('output/rk2_1.00.csv')

rk4_001 = load_csv('output/rk4_0.01.csv')
rk4_010 = load_csv('output/rk4_0.10.csv')
rk4_100 = load_csv('output/rk4_1.00.csv')

def plot_solution_and_error(analytic_001, analytic_010, analytic_100, num_001, num_010, num_100, method_name, zoom=False):
    t_analytic = analytic_001[:, 0]
    analytic_solution = analytic_001[:, 1]

    # y_num - y_dok
    error_001 = num_001[:, 1] - analytic_001[:, 1]
    error_010 = num_010[:, 1] - analytic_010[:, 1]
    error_100 = num_100[:, 1] - analytic_100[:, 1]

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))

    # result
    ax1.plot(t_analytic, analytic_solution, label="analityczne", color="black")
    ax1.scatter(num_001[:, 0], num_001[:, 1], color="blue", label="dt=0.01", s=10)
    ax1.scatter(num_010[:, 0], num_010[:, 1], color="red", label="dt=0.1", s=10)
    ax1.scatter(num_100[:, 0], num_100[:, 1], color="green", label="dt=1.0", s=10)

    ax1.set_xlabel("t")
    ax1.set_ylabel("y(t)")
    ax1.set_title(f"{method_name} - rozwiązanie")
    ax1.legend()

    # error
    ax2.plot(num_001[:, 0], error_001, label="dt=0.01", color="blue")
    ax2.plot(num_010[:, 0], error_010, label="dt=0.1", color="red")
    ax2.plot(num_100[:, 0], error_100, label="dt=1.0", color="green")

    ax2.set_xlabel("t")
    ax2.set_ylabel("$y_{num}(t) - y_{dok}(t)$")
    ax2.set_title(f"{method_name} - błąd globalny")
    ax2.legend()

    # zoom
    if method_name == "Metoda RK2" or method_name == "Metoda RK4":
        # Create inset axes inside the second plot
        axins = ax2.inset_axes([0.62, 0.5, 0.2, 0.4])
        axins.plot(num_001[:, 0], error_001, color="blue")
        axins.plot(num_010[:, 0], error_010, color="red")
        axins.plot(num_100[:, 0], error_100, color="green")
        
        # different limits for each of the plots
        if method_name == "Metoda RK2":
            axins.set_ylim(0, 0.001)
        elif method_name == "Metoda RK4":
            axins.set_ylim(0, 9e-7)
        
        axins.set_xlim(0, 4)

    plt.tight_layout()
    save_plot(fig, f"{method_name.replace(' ', '_')}_porownanie.png")

# zad. 4
def plot_qt_it(qt_it_data):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 10))

    # Q(t)
    for file_path, label in qt_it_data:
        data = load_csv(file_path)
        ax1.plot(data[:, 0], data[:, 1], label=label)

    ax1.set_title("z.4 - Metoda RK4, Q(t)")
    ax1.set_ylabel("Q(t)")
    ax1.legend()

    # I(t)
    for file_path, label in qt_it_data:
        data = load_csv(file_path)
        ax2.plot(data[:, 0], data[:, 2], label=label)

    ax2.set_title("z.4 - Metoda RK4, I(t)")
    ax2.set_xlabel("Czas (t)")
    ax2.set_ylabel("I(t)")
    ax2.legend()

    plt.tight_layout()
    save_plot(fig, "Qt_It.png")

qt_it_data = [
    ('output/rlc_50.00.csv', '$0.5ω_0$'),
    ('output/rlc_80.00.csv', '$0.8ω_0$'),
    ('output/rlc_100.00.csv', '$1.0ω_0$'),
    ('output/rlc_120.00.csv', '$1.2ω_0$')
]

plot_solution_and_error(analytic_001, analytic_010, analytic_100, euler_001, euler_010, euler_100, "Metoda Eulera")
plot_solution_and_error(analytic_001, analytic_010, analytic_100, rk2_001, rk2_010, rk2_100, "Metoda RK2", zoom=True)
plot_solution_and_error(analytic_001, analytic_010, analytic_100, rk4_001, rk4_010, rk4_100, "Metoda RK4", zoom=True)
plot_qt_it(qt_it_data)