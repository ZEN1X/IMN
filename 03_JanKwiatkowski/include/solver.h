//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#ifndef SOLVER_H
#define SOLVER_H

#include "types.h"

vec_step trapeze(const vec_step& curr_vec, double dt, double alpha);

vec_step rk2(const vec_step& curr_vec, double dt, double alpha);

#endif  // SOLVER_H
