//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#include "time_control.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include "data.h"

void time_control(solve_func f,
                  const std::string& filename,
                  const double TOL,
                  const double x0,
                  const double v0,
                  const double dt0,
                  const double S,
                  const double p,
                  const double t_max,
                  const double alpha) {
  // initialize
  double t = 0;
  double dt = dt0;
  vec_step vec_curr{x0, v0};

  // prepare file for saving
  std::ofstream file{filename};
  file << std::fixed << std::setprecision(18);

  do {
    // dwa kroki dt
    auto next_1 = f(vec_curr, dt, alpha);
    auto next_2 = f(next_1, dt, alpha);

    // jeden krok 2dt
    auto next_twice = f(vec_curr, 2 * dt, alpha);

    // calculate Ex, Ey
    double denominator = std::pow(2, p) - 1;
    double Ex = (next_2.first - next_twice.first) / denominator;
    double Ey = (next_2.second - next_twice.second) / denominator;

    if (std::max(std::abs(Ex), std::abs(Ey)) < TOL) {
      t += 2 * dt;
      vec_curr = next_2;
      // save
      save_data(file, vec_curr, t, dt);
    }

    double base = S * TOL / std::max(std::abs(Ex), std::abs(Ey));
    double expo = 1.0 / (p + 1.0);
    dt *= std::pow(base, expo);
  } while (t < t_max);

  file.close();
}
