#include "convection-diffusion.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>

v_field::v_field(const comp_grid& psi) : x{}, y{} {
  for (int i = 1; i < NX; ++i) {
    for (int j = 1; j < NY; j++) {
      x[i][j] = (psi[i][j + 1] - psi[i][j - 1]) / (2.0 * DELTA);
      y[i][j] = -(psi[i + 1][j] - psi[i - 1][j]) / (2.0 * DELTA);
    }
  }

  // blockade
  for (int i = I1; i <= I2; ++i) {
    for (int j = 0; j <= J1; j++) {
      x[i][j] = y[i][j] = 0;
    }
  }

  // top & bottom edge
  for (int i = 1; i < NX; ++i) {
    x[i][0] = y[i][NY] = 0;
  }

  // left & right edge
  for (int j = 0; j <= NY; ++j) {
    x[0][j] = x[1][j];
    x[NX][j] = x[NX - 1][j];
  }
}

stats::stats(const comp_grid& u) : c{0.0}, x_avg{0.0} {
  for (int i = 0; i <= NX; ++i) {
    for (int j = 0; j <= NY; ++j) {
      c += u[i][j];

      double x = DELTA * i;
      x_avg += x * u[i][j];
    }
  }

  // multiply now, to reduce floating point errors
  c *= DELTA * DELTA;
  x_avg *= DELTA * DELTA;
}

auto load_psi() -> comp_grid {
  comp_grid psi{};
  std::ifstream psi_file{"data/psi.dat"};

  // check if file opened succesfully
  if (!psi_file) {
    throw std::runtime_error(
        "Couldn't open the psi.dat file under the data/ directory.");
  }

  int i, j;
  double val;
  while (psi_file >> i >> j >> val) {
    psi[i][j] = val;
  }

  return psi;
}

auto calc_vmax(const v_field& v) -> double {
  double vmax = 0;

  for (int i = 0; i <= NX; ++i) {
    for (int j = 0; j <= NY; ++j) {
      double v_magnitude =
          std::sqrt(std::pow(v.x[i][j], 2) + std::pow(v.y[i][j], 2));

      vmax = std::max(vmax, v_magnitude);
    }
  }

  return vmax;
}

auto u_init() -> comp_grid {
  comp_grid u{};

  for (int i = 0; i <= NX; ++i) {
    for (int j = 0; j <= NY; ++j) {
      double x = DELTA * i;
      double y = DELTA * j;
      u[i][j] = 1.0 / (2 * M_PI * SIGMA * SIGMA) *
                std::exp(-(std::pow(x - XA, 2) + std::pow(y - YA, 2)) /
                         (2 * SIGMA * SIGMA));
    }
  }

  return u;
}

auto solve(double D) -> void {
  comp_grid u0{u_init()}, u1{}, psi{load_psi()};
  v_field v{psi};

  double vmax = calc_vmax(v);
  double dt = DELTA / (4.0 * vmax);

  // c(t), x_avg(t), u(x,y) filenames
  std::string stats_filename{"ct_d"};
  std::string u_filename{"u_t_d"};
  if (D == 0) {
    stats_filename += "0";
    u_filename += "0";
  } else if (D == 0.1) {
    stats_filename += "01";
    u_filename += "01";
  }
  stats_filename += ".dat";
  u_filename += ".dat";

  // c(t), x_avg(t), u(x,y) files
  std::ofstream stats_file{stats_filename};
  std::ofstream u_file{u_filename};

  for (int it = 1; it <= IT_MAX; ++it) {
    // Picard start
    u1 = u0;

    for (int k = 1; k <= 20; ++k) {
      for (int i = 0; i <= NX; ++i) {
        for (int j = 1; j <= NY; ++j) {
          if (i >= I1 && i <= I2 && j <= J1) {  // blockade
            continue;

          } else if (i == 0) {  // left edge
            u1[i][j] =
                (1 / (1 + ((2 * D * dt) / std::pow(DELTA, 2)))) *
                (u0[i][j] -
                 (dt / 2) * v.x[i][j] *
                     (((u0[i + 1][j] - u0[NX][j]) / (2 * DELTA)) +
                      (u1[i + 1][j] - u1[NX][j]) / (2 * DELTA)) -
                 (dt / 2) * v.y[i][j] *
                     ((u0[i][j + 1] - u0[i][j - 1]) / (2 * DELTA) +
                      (u1[i][j + 1] - u1[i][j - 1]) / (2 * DELTA)) +
                 (dt / 2) * D *
                     ((u0[i + 1][j] + u0[NX][j] + u0[i][j + 1] + u0[i][j - 1] -
                       4 * u0[i][j]) /
                          std::pow(DELTA, 2) +
                      (u1[i + 1][j] + u1[NX][j] + u1[i][j + 1] + u1[i][j - 1]) /
                          std::pow(DELTA, 2)));

          } else if (i == NX) {  // right edge
            u1[i][j] =
                (1 / (1 + ((2 * D * dt) / std::pow(DELTA, 2)))) *
                (u0[i][j] -
                 (dt / 2) * v.x[i][j] *
                     (((u0[0][j] - u0[i - 1][j]) / (2 * DELTA)) +
                      (u1[0][j] - u1[i - 1][j]) / (2 * DELTA)) -
                 (dt / 2) * v.y[i][j] *
                     ((u0[i][j + 1] - u0[i][j - 1]) / (2 * DELTA) +
                      (u1[i][j + 1] - u1[i][j - 1]) / (2 * DELTA)) +
                 (dt / 2) * D *
                     ((u0[0][j] + u0[i - 1][j] + u0[i][j + 1] + u0[i][j - 1] -
                       4 * u0[i][j]) /
                          std::pow(DELTA, 2) +
                      (u1[0][j] + u1[i - 1][j] + u1[i][j + 1] + u1[i][j - 1]) /
                          std::pow(DELTA, 2)));

          } else {  // inside (aka everywhere else)
            u1[i][j] = (1 / (1 + ((2 * D * dt) / std::pow(DELTA, 2)))) *
                       (u0[i][j] -
                        (dt / 2) * v.x[i][j] *
                            (((u0[i + 1][j] - u0[i - 1][j]) / (2 * DELTA)) +
                             (u1[i + 1][j] - u1[i - 1][j]) / (2 * DELTA)) -
                        (dt / 2) * v.y[i][j] *
                            ((u0[i][j + 1] - u0[i][j - 1]) / (2 * DELTA) +
                             (u1[i][j + 1] - u1[i][j - 1]) / (2 * DELTA)) +
                        (dt / 2) * D *
                            ((u0[i + 1][j] + u0[i - 1][j] + u0[i][j + 1] +
                              u0[i][j - 1] - 4 * u0[i][j]) /
                                 std::pow(DELTA, 2) +
                             (u1[i + 1][j] + u1[i - 1][j] + u1[i][j + 1] +
                              u1[i][j - 1]) /
                                 std::pow(DELTA, 2)));
          }
        }
      }
    }
    // Picard end
    u0 = u1;

    // calc and save c & x_sr
    stats sts{u0};
    stats_file << dt * it << '\t' << sts.c << '\t' << sts.x_avg << '\n';

    // save u(x,y) to file
    if ((it % 1000 == 0) && it != 0) {
      for (int i = 0; i <= NX; ++i) {
        for (int j = 0; j <= NY; ++j) {
          u_file << i << '\t' << j << '\t' << u0[i][j] << '\n';
        }
        u_file << '\n';
      }
      u_file << '\n';
    }
  }
}

auto save_v_field() -> void {
  comp_grid psi{load_psi()};
  v_field v{psi};

  std::ofstream v_file{"v.dat"};

  for (int i = 0; i <= NX; ++i) {
    for (int j = 0; j <= NY; ++j) {
      v_file << i << '\t' << j << '\t' << v.x[i][j] << '\t' << v.y[i][j]
             << '\n';
    }
    v_file << '\n';
  }
}
