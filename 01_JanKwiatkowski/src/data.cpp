#include "data.h"
#include <iomanip>

void save_data(const std::pair<std::vector<double>, std::vector<double>>& data, std::ostream& out) {
  out << std::fixed << std::setprecision(14);
  for (std::size_t i = 0; i < data.first.size(); i++) {
    out << data.first[i] << "," << data.second[i] << "\n";
  }
}

std::pair<std::vector<double>, std::vector<double>> plot_func(std::function<double(double)> f, double from, double to, double step) {
  double x = from;
  std::vector<double> x_vec;
  std::vector<double> y_vec;

  while (x <= to) {
    x_vec.push_back(x);
    y_vec.push_back(f(x));
    x += step;
  }

  return {x_vec, y_vec};
}

std::vector<double> calc_error(const std::vector<double>& first, const std::vector<double>& second) {
  std::vector<double> error;

  for (std::size_t i = 0; i < first.size(); i++) {
    error.push_back(first[i] - second[i]);
  }

  return error;
}
