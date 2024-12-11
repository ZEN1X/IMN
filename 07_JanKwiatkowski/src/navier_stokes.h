#ifndef NAVIER_STOKES_H
#define NAVIER_STOKES_H

#include <array>

// constants
constexpr double DELTA = 0.01;
constexpr double RHO = 1.0;
constexpr double MU = 1.0;
constexpr int NX = 200;
constexpr int NY = 90;
constexpr int I1 = 50;
constexpr int J1 = 55;
constexpr int J2 = J1 + 2;
constexpr int IT_MAX = 20000;

using comp_grid = std::array<std::array<double, NY + 1>, NX + 1>;

#endif /* NAVIER_STOKES_H */
