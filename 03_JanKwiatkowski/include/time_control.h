//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#ifndef TIME_CONTROL_H
#define TIME_CONTROL_H

#include <string>
#include "types.h"

void time_control(solve_func f,
                  const std::string& filename,
                  double TOL,
                  double x0 = 0.01,
                  double v0 = 0.0,
                  double dt0 = 1.0,
                  double S = 0.75,
                  double p = 2.0,
                  double t_max = 40.0,
                  double alpha = 5.0);

#endif  // TIME_CONTROL_H
