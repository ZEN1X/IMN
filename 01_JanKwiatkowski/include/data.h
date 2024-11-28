#ifndef DATA_H
#define DATA_H

#include <ostream>
#include <utility>
#include <vector>
#include <functional>

void save_data(const std::pair<std::vector<double>, std::vector<double>>& data, std::ostream& out);

std::pair<std::vector<double>, std::vector<double>> plot_func(std::function<double(double)> f, double from, double to, double step);

std::vector<double> calc_error(const std::vector<double>& first, const std::vector<double>& second);

#endif /* DATA_H */
