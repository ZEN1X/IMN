//
// Created by Jan Kwiatkowski on 01/12/2024.
//

#ifndef ALGEBRAIC_SOLVER_H
#define ALGEBRAIC_SOLVER_H

#include <array>
#include "types.h"

double rho(double x, double y, double xmax, double ymax, double sigma);

double epsilon(int l, int nx, double epsilon1, double epsilon2);

inline int get_j(const int l, const int nx) {
    return l / (nx + 1);
}

inline int get_i(const int l, const int nx) {
    return l - get_j(l, nx) * (nx + 1);
}

init_vectors initialize_vectors(int nx, int ny, const std::array<double, 4> &Vs, bool nonzero_rho, double epsilon1,
                                double epsilon2);

void solve(init_vectors &data, const std::string &filename);

#endif //ALGEBRAIC_SOLVER_H
