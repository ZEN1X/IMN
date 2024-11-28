#ifndef EULER_H
#define EULER_H

#include <functional>
#include <utility>
#include <vector>

std::pair<std::vector<double>, std::vector<double>> euler(double y0, double dt, std::function<double(double, double)> f, double t_from, double t_to);

#endif /* EULER_H */
