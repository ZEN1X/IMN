//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#include "solver.h"
#include <cmath>

vec_step trapeze(const vec_step& curr_vec,
                 const double dt,
                 const double alpha) {
  static constexpr double DELTA{1e-10};

  // aids with notation
  const double x_n{curr_vec.first};
  const double v_n{curr_vec.second};

  // initially
  double x_next = x_n, v_next = v_n;

  // access dx and dv in the while() condition
  double dx{};
  double dv{};

  do {
    // F & G
    const double F = x_next - x_n - dt / 2.0 * (v_n + v_next);
    auto g = [alpha](const double x, const double v) -> double {
      return alpha * (1 - x * x) * v - x;
    };
    const double G =
        v_next - v_n - dt / 2.0 * (g(x_n, v_n) + g(x_next, v_next));

    constexpr double a11 = 1;
    const double a12 = -dt / 2.0;
    const double a21 = -dt / 2.0 * (-2 * alpha * x_next * v_next - 1);
    const double a22 = 1 - dt / 2.0 * alpha * (1 - x_next * x_next);

    // determinant method
    const double denominator = a11 * a22 - a12 * a21;
    dx = (-F * a22 + G * a12) / denominator;
    dv = (a11 * -G + a21 * F) / denominator;

    // new x and v
    x_next += dx;
    v_next += dv;
  } while (std::abs(dx) > DELTA || std::abs(dv) > DELTA);

  // loop finished, we got our results
  return {x_next, v_next};
}

vec_step rk2(const vec_step& curr_vec, const double dt, const double alpha) {
  // aids with notation
  const double x_n{curr_vec.first};
  const double v_n{curr_vec.second};

  // define g-lambda
  auto g = [alpha](const double x, const double v) -> double {
    return alpha * (1 - x * x) * v - x;
  };

  const double k1x = v_n;
  const double k1v = g(x_n, v_n);

  const double k2x = v_n + dt * k1v;
  const double k2v = g(x_n + dt * k1x, v_n + dt * k1v);

  const double x_next = x_n + dt / 2.0 * (k1x + k2x);
  const double v_next = v_n + dt / 2.0 * (k1v + k2v);

  return {x_next, v_next};
}
