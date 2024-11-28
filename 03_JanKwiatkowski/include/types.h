//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#ifndef TYPES_H
#define TYPES_H

#include <utility>

using vec_step = std::pair<double, double>;
using solve_func = vec_step (*)(const vec_step&, double, double);

#endif  // TYPES_H
