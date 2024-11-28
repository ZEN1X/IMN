#ifndef IMPLICIT_RK2_H
#define IMPLICIT_RK2_H

#include "types.h"

function_plot implicit_rk2(const two_valued_func& func, const two_valued_func& derivative, double t_step = 0.1, double t_max = 100, double u_0 = 1, double tolerance = 1e-6, double mu_max = 20);

#endif /* IMPLICIT_RK2_H */
