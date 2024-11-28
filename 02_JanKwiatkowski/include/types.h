#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <utility>
#include <vector>

using function_plot = std::vector<std::pair<double, double>>;
using two_valued_func = std::function<double(double, double)>;

#endif /* TYPES_H */
