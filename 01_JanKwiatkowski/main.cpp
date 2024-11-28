#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "data.h"
#include "euler.h"
#include "rk2.h"
#include "rk4.h"

auto func = [](double t, double y) {
  return -y;
};

double dt_arr[] = {0.01, 0.1, 1.0};

void gen_analytic_func() {
  for (double dt : dt_arr) {
    auto analytic_func = [](double t) -> double {
      return std::exp(-t);
    };
    auto analytic_data{plot_func(analytic_func, 0, 5, dt)};

    std::ostringstream filename;
    filename << "analytic_" << std::fixed << std::setprecision(2) << dt << ".csv";
    std::ofstream file(filename.str());
    save_data(analytic_data, file);
    file.close();
  }
}

void ex1() {
  for (double dt : dt_arr) {
    auto euler_data{euler(1, dt, func, 0, 5)};

    std::ostringstream filename;
    filename << "euler_" << std::fixed << std::setprecision(2) << dt << ".csv";

    // save raw data
    std::ofstream file(filename.str());
    save_data(euler_data, file);
    file.close();
  }
}

void ex2() {
  for (double dt : dt_arr) {
    auto rk2_data{rk2(1, dt, func, 0, 5)};

    std::ostringstream filename;
    filename << "rk2_" << std::fixed << std::setprecision(2) << dt << ".csv";

    // save raw data
    std::ofstream file(filename.str());
    save_data(rk2_data, file);
    file.close();
  }
}

void ex3() {
  for (double dt : dt_arr) {
    std::array<double, 1> s = {1};  // initial conditions
    double t{0};

    auto func_vec = [](double t, const std::array<double, 1>& s, std::array<double, 1>& k) {
      k[0] = func(t, s[0]);
    };  // "wrapper"

    std::ostringstream filename;
    filename << "rk4_" << std::fixed << std::setprecision(2) << dt << ".csv";

    // save initial values
    std::ofstream file(filename.str());
    file << std::fixed << std::setprecision(14);
    file << t << "," << s[0] << "\n";

    while (t <= 5 - dt) {
      rk4_vec(t, dt, s, func_vec);
      t += dt;

      // saving to file
      file << t << "," << s[0] << "\n";
    }

    file.close();
  }
}

void ex4() {
  // constants
  const double R = 100.0;
  const double L = 0.1;
  const double C = 0.001;
  const double omega_0 = 1.0 / std::sqrt(L * C);
  const double T0 = 2 * M_PI / omega_0;
  const double dt = 1e-4;
  const double t_max = 4 * T0;

  auto V = [](double t, double omega_V) {
    return 10 * std::sin(omega_V * t);
  };

  std::vector<double> omega_V_arr = {0.5 * omega_0, 0.8 * omega_0, 1.0 * omega_0, 1.2 * omega_0};

  for (double omega_V : omega_V_arr) {
    std::array<double, 2> s = {0.0, 0.0};  // initial conditions
                                           // remember to reset for each iteration :)

    std::ostringstream filename;
    filename << "rlc_" << std::fixed << std::setprecision(2) << omega_V << ".csv";
    std::ofstream file(filename.str());

    double t = 0.0;

    auto rlc_eq = [R, L, C, V, omega_V](double t, const std::array<double, 2>& s, std::array<double, 2>& k) {
      // helper variables to make the formulas easier
      double Q = s[0];                                                 // Q(t)
      double I = s[1];                                                 // I(t)
      k[0] = I;                                                        // dQ/dt = I
      k[1] = (V(t, omega_V) / L) - (R / L) * I - (1.0 / (L * C)) * Q;  // dI/dt
    };

    // save initial values
    file << t << "," << s[0] << "," << s[1] << "\n";

    while (t <= t_max) {
      rk4_vec(t, dt, s, rlc_eq);
      t += dt;

      file << t << "," << s[0] << "," << s[1] << "\n";
    }

    file.close();
  }
}

int main(int argc, char const* argv[]) {
  gen_analytic_func();
  ex1();
  ex2();
  ex3();
  ex4();

  return 0;
}
