#ifndef TRAPEZE_H
#define TRAPEZE_H

#include "types.h"
#include <fstream>

/**
 * @brief Solves a differential equation using trapeze method with either Picard's or Newton's iteration.
 * 
 * @param func function which describes the differential equation
 * @param derivative derivative of the function w.r.t. u
 * @param iter_type specifies whether to use Picard's or Newton's iteration; false = Picard's, true = Newton's
 * @param t_step time step
 * @param t_max limit of solution
 * @param u_0 starting value
 * @param tolerance tolerance for when to stop iterating
 * @param mu_max limit of iterations
 * @return function_plot
 */
function_plot trapeze(const two_valued_func& func, const two_valued_func& derivative, bool iter_type = false, double t_step = 0.1, double t_max = 100, double u_0 = 1, double tolerance = 1e-6, double mu_max = 20);

/**
 * @brief Performs Picard's iteration.
 * 
 * @param func function which describes the differential equation
 * @param u_n function value, for which to perform iteration
 * @param t_prev previous time value (aka t_n)
 * @param t time for current calculation (t_n+1)
 * @param t_step time step
 * @param tolerance tolerance for when to stop iterating
 * @param mu_max limit of iterations
 * @param mu_out file to output the no of iterations
 * @return iterated function value 
 */
double picard_iter(const two_valued_func& func, double u_n, double t_prev, double t, double t_step, double tolerance, double mu_max, std::ofstream& mu_out);

/**
 * @brief Performs Newton's iteration.
 * 
 * @param func function which describes the differential equation
 * @param derivative derivative of the function w.r.t. u
 * @param u_n function value, for which to perform iteration
 * @param t_prev previous time value (aka t_n)
 * @param t time for current calculation (t_n+1)
 * @param t_step time step
 * @param tolerance tolerance for when to stop iterating
 * @param mu_max limit of iterations
 * @param mu_out file to output the no of iterations
 * @return iterated function value 
 */
double newton_iter(const two_valued_func& func, const two_valued_func& derivative, double u_n, double t_prev, double t, double t_step, double tolerance, double mu_max, std::ofstream& mu_out);

#endif /* TRAPEZE_H */
