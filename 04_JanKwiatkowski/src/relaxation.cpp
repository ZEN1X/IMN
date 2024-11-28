#include "relaxation.h"
#include <cmath>

#include <iostream>

comp_grid generate_init_cond(void) {
  comp_grid V{};  // zero-initializes cuz double is POD
  std::size_t rows = V.size();

// #pragma omp parallel for
  for (std::size_t col = 0; col < V[0].size(); ++col) {
    V[0][col] = V2;
    V[rows - 1][col] = V1;
  }

  return V;
}

double calc_S(const comp_grid& V, const comp_grid& density) {
  double S = 0;

  std::size_t rows = V.size();
  std::size_t cols = V[0].size();

// #pragma omp parallel for reduction(+ : S)
  for (std::size_t row = 0; row < rows - 1; ++row) {    // row -> j
    for (std::size_t col = 0; col < cols - 1; ++col) {  // col -> i
      S += DELTA * DELTA *
           (0.5 * std::pow((V[row][col + 1] - V[row][col]) / (DELTA), 2) +
            0.5 * std::pow((V[row + 1][col] - V[row][col]) / (DELTA), 2) -
            density[row][col] * V[row][col]);
    }
  }

  return S;
}

std::pair<comp_grid, std::vector<double>> global_relax(const comp_grid& density,
                                                       double omega_g,
                                                       double tolerance) {
  comp_grid Vs = generate_init_cond();
  comp_grid Vn = generate_init_cond();
  std::vector<double> S_vec{calc_S(Vs, density)};

  std::size_t rows = Vs.size();
  std::size_t cols = Vs[0].size();

  double stop;
  do {
// #pragma omp parallel for collapse(2)
    // 1) all but edge
    for (std::size_t row = 1; row < rows - 1; ++row) {    // row -> j
      for (std::size_t col = 1; col < cols - 1; ++col) {  // col -> i
        Vn[row][col] = 0.25 * (Vs[row][col + 1] + Vs[row][col - 1] +
                               Vs[row + 1][col] + Vs[row - 1][col] +
                               DELTA * DELTA / EPSILON * density[row][col]);
      }
    }

// #pragma omp parallel for
    // 2) Neumann
    for (std::size_t row = 1; row < rows - 1; ++row) {  // row -> j
      Vn[row][0] = Vn[row][1];
      Vn[row][cols - 1] = Vn[row][cols - 2];
      // cols - 1 is the last index, cols - 2 is second last
    }

// #pragma omp parallel for collapse(2)
    // 3) Mix
    for (std::size_t row = 0; row < rows; ++row) {    // row -> j
      for (std::size_t col = 0; col < cols; ++col) {  // col -> i
        Vs[row][col] = (1.0 - omega_g) * Vs[row][col] + omega_g * Vn[row][col];
      }
    }

    double S = calc_S(Vs, density);

    stop = std::abs((S - S_vec.back()) / (S_vec.back()));

    S_vec.push_back(S);
  } while (stop > tolerance);

  return {Vs, S_vec};
}

std::pair<comp_grid, std::vector<double>> local_relax(const comp_grid& density,
                                                      double omega_l,
                                                      double tolerance) {
  comp_grid V = generate_init_cond();
  std::vector<double> S_vec{calc_S(V, density)};

  std::size_t rows = V.size();
  std::size_t cols = V[0].size();

  double stop;
  do {
//#pragma omp parallel for collapse(2)
    // 1) all but edge
    for (std::size_t col = 1; col < cols - 1; ++col) {    // row -> j
      for (std::size_t row = 1; row < rows - 1; ++row) {  // col -> i
        V[row][col] =
            (1.0 - omega_l) * V[row][col] +
            omega_l * 0.25 *
                (V[row][col + 1] + V[row][col - 1] + V[row + 1][col] +
                 V[row - 1][col] + DELTA * DELTA / EPSILON * density[row][col]);
      }
    }

    // for (std::size_t row = 1; row < rows - 1; ++row) {    // row -> j
    //   for (std::size_t col = 1; col < cols - 1; ++col) {  // col -> i
    //     V[row][col] =
    //         (1.0 - omega_l) * V[row][col] +
    //         omega_l * 0.25 *
    //             (V[row][col + 1] + V[row][col - 1] + V[row + 1][col] +
    //              V[row - 1][col] + DELTA * DELTA / EPSILON *
    //              density[row][col]);
    //   }
    // }

//#pragma omp parallel for
    // 2) Neumann
    for (std::size_t row = 1; row < rows - 1; ++row) {  // row -> j
      V[row][0] = V[row][1];
      V[row][cols - 1] = V[row][cols - 2];
      // cols - 1 is the last index, cols - 2 is second last
    }

    double S = calc_S(V, density);

    stop = std::abs((S - S_vec.back()) / (S_vec.back()));

    S_vec.push_back(S);
  } while (stop > tolerance);

  return {V, S_vec};
}

comp_grid calc_error(const comp_grid& V, const comp_grid& density) {
  comp_grid error{};

  std::size_t rows = V.size();
  std::size_t cols = V[0].size();

// #pragma omp parallel for collapse(2)
  // interior
  for (std::size_t row = 1; row < rows - 1; ++row) {
    for (std::size_t col = 1; col < cols - 1; ++col) {
      error[row][col] = (V[row][col + 1] + V[row][col - 1] + V[row + 1][col] +
                         V[row - 1][col] - 4 * V[row][col]) /
                            (DELTA * DELTA) +
                        density[row][col] / EPSILON;
    }
  }

// #pragma omp parallel for
  // exterior (need to take into account the neumann boundary cond)
  for (std::size_t row = 1; row < rows - 1; ++row) {  // row -> j
    error[row][0] = error[row][1];
    error[row][cols - 1] = error[row][cols - 2];
    // cols - 1 is the last index, cols - 2 is second last
  }

  return error;
}
