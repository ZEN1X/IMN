#ifndef RK2_H
#define RK2_H

#include <functional>
#include <utility>
#include <vector>

std::pair<std::vector<double>, std::vector<double>> rk2(double y0, double dt, std::function<double(double, double)> f, double t_from, double t_to);

#endif /* RK2_H */
