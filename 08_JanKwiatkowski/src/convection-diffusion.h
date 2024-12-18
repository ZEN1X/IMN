#ifndef CONVECTION_DIFFUSION_H
#define CONVECTION_DIFFUSION_H

#include <array>

// constants
constexpr int NX = 400;
constexpr int NY = 90;
constexpr int I1 = 200;
constexpr int I2 = 210;
constexpr int J1 = 50;
constexpr double DELTA = 0.01;
constexpr double SIGMA = 10 * DELTA;
constexpr double XA = 0.45;
constexpr double YA = 0.45;
constexpr int IT_MAX = 10'000;

// typedefs
using row = std::array<double, NY + 1>;
using comp_grid = std::array<row, NX + 1>;

// v-field struct
struct v_field {
  comp_grid x;
  comp_grid y;

  v_field(const comp_grid& psi);
};

// c & x_avg
struct stats {
  double c;
  double x_avg;

  stats(const comp_grid& u);
};

// functions
auto load_psi() -> comp_grid;
auto calc_vmax(const v_field& v) -> double;
auto u_init() -> comp_grid;
auto solve(double D) -> void;
auto save_v_field() -> void;

#endif /* CONVECTION_DIFFUSION_H */
