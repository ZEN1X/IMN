#include "trapeze.h"
#include <cmath>

function_plot trapeze(const two_valued_func& func, const two_valued_func& derivative, bool iter_type, double t_step, double t_max, double u_0, double tolerance, double mu_max) {
  function_plot result;

  // mu output
  std::ofstream mu_out;
  if (iter_type == false) {
    mu_out.open("picard_mu.csv");
  } else {
    mu_out.open("newton_mu.csv");
  }

  // starting values
  result.emplace_back(0.0, u_0);

  // start calculation
  double t = t_step;  // 0.0, u_0 already added previously, start from here
  while (t <= t_max) {
    double t_prev = result.back().first;
    double u_prev = result.back().second;

    // begin iteration
    double u_new;
    if (iter_type == false) {
      u_new = picard_iter(func, u_prev, t_prev, t, t_step, tolerance, mu_max, mu_out);
    } else {
      u_new = newton_iter(func, derivative, u_prev, t_prev, t, t_step, tolerance, mu_max, mu_out);
    }

    // iteration finished, append the calculation to the result
    result.emplace_back(t, u_new);

    t += t_step;
  }

  mu_out.close();

  return result;
}

double picard_iter(const two_valued_func& func, double u_n, double t_prev, double t, double t_step, double tolerance, double mu_max, std::ofstream& mu_out) {
  int mu = 0;
  double u_curr_iter = u_n;
  double u_old_iter;
  do {
    u_old_iter = u_curr_iter;
    u_curr_iter = u_n + t_step / 2.0 * (func(t_prev, u_n) + func(t, u_curr_iter));

    mu++;
  } while ((mu <= mu_max) && (std::abs(u_curr_iter - u_old_iter) > tolerance));

  mu_out << t << ',' << mu << '\n';

  return u_curr_iter;
}

double newton_iter(const two_valued_func& func, const two_valued_func& derivative, double u_n, double t_prev, double t, double t_step, double tolerance, double mu_max, std::ofstream& mu_out) {
  int mu = 0;
  double u_curr_iter = u_n;
  double u_old_iter;
  do {
    u_old_iter = u_curr_iter;
    // u_curr_iter = u_n + t_step / 2.0 * (func(t_prev, u_n) + func(t, u_curr_iter));
    u_curr_iter = u_old_iter - (u_old_iter - u_n - t_step / 2.0 * (func(t_prev, u_n) + func(t, u_curr_iter))) / (1 - t_step / 2.0 * derivative(t, u_old_iter));

    mu++;
  } while ((mu <= mu_max) && (std::abs(u_curr_iter - u_old_iter) > tolerance));

  mu_out << t << ',' << mu << '\n';

  return u_curr_iter;
}
