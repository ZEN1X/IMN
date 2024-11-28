#include "implicit_rk2.h"
#include <cmath>
#include <fstream>

function_plot implicit_rk2(const two_valued_func& func, const two_valued_func& derivative, double t_step, double t_max, double u_0, double tolerance, double mu_max) {
  function_plot result;

  std::ofstream mu_out("rk2_mu.csv");

  // define constants from the Butcher's tablueau
  const double a11 = 0.25, a22 = a11;
  const double a12 = 0.25 - std::sqrt(3) / 6.0;
  const double a21 = 0.25 + std::sqrt(3) / 6.0;

  const double b1 = 0.5, b2 = b1;

  const double c1 = 0.5 - std::sqrt(3) / 6.0;
  const double c2 = 0.5 + std::sqrt(3) / 6.0;

  // starting values
  result.emplace_back(0.0, u_0);

  // start calculation
  double t = t_step;  // 0.0, u_0 already added previously, start from here
  while (t <= t_max) {
    double t_prev = result.back().first;
    double u_prev = result.back().second;

    // // begin iteration
    // double u_new;
    // if (iter_type == false) {
    //   u_new = picard_iter(func, u_prev, t_prev, t, t_step, tolerance, mu_max);
    // } else {
    //   u_new = newton_iter(func, derivative, u_prev, t_prev, t, t_step, tolerance, mu_max);
    // }

    int mu = 0;
    double u_curr_iter = u_prev;
    double u_old_iter;

    double U1 = u_curr_iter;
    double U2 = u_curr_iter;

    double delta_U1;
    double delta_U2;

    u_old_iter = u_curr_iter;
    do {
      double F1 = U1 - u_curr_iter - t_step * (a11 * func(t_prev, U1) + a12 * func(t_prev, U2));
      double F2 = U2 - u_curr_iter - t_step * (a21 * func(t_prev, U1) + a22 * func(t_prev, U2));

      double m11 = 1 - t_step * a11 * derivative(t_prev, U1);
      double m12 = -t_step * a12 * derivative(t_prev, U2);
      double m21 = -t_step * a21 * derivative(t_prev, U1);
      double m22 = 1 - t_step * a22 * derivative(t_prev, U2);

      delta_U1 = (F2 * m12 - F1 * m22) / (m11 * m22 - m12 * m21);
      delta_U2 = (F1 * m21 - F2 * m11) / (m11 * m22 - m12 * m21);

      U1 += delta_U1;
      U2 += delta_U2;

      // u_curr_iter = u_old_iter + t_step * (b1 * func(t_prev + c1 * t_step, U1 + b2 * func(t_prev + c2 * t_step, U2)));

      mu++;
    } while ((mu <= mu_max) && ((std::abs(delta_U1) > tolerance) || (std::abs(delta_U2) > tolerance)));
    u_curr_iter = u_old_iter + t_step * (b1 * func(t_prev + c1 * t_step, U1) + b2 * func(t_prev + c2 * t_step, U2));

    // iteration finished, append the calculation to the result
    result.emplace_back(t, u_curr_iter);

    t += t_step;

    mu_out << t << ',' << mu << '\n';
  }

  return result;
}