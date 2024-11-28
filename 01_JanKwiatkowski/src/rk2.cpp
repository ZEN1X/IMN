#include "rk2.h"

std::pair<std::vector<double>, std::vector<double>> rk2(double y0, double dt, std::function<double(double, double)> f, double t_from, double t_to) {
  std::vector<double> t_vector;
  std::vector<double> y_vector;

  // write the initial conditions to the output vectors
  t_vector.push_back(t_from);
  y_vector.push_back(y0);

  double t = t_from + dt;  // since we've already pushed the i.c., we start from t + dt;

  while (t <= t_to) {
    t_vector.push_back(t);
    double y{y_vector.back()};

    double k1{f(t, y)};
    double k2{f(t + dt, y + dt * k1)};

    double y_new{y + dt / 2.0 * (k1 + k2)};
    y_vector.push_back(y_new);

    t += dt;
  }

  return {t_vector, y_vector};
}