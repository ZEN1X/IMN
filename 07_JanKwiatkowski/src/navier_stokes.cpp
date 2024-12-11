#include "navier_stokes.h"
#include <cmath>

/**
 * @brief Calculate Q_out based on Q_in.
 *
 * @param Q_in
 * @return double, calculated Q_out
 */
double Q_out(double Q_in) {
  return Q_in * std::pow((NY - J1) / NY, 3);
}

/**
 * @brief Check whether a point on the grid falls on an edge.
 *
 * @param i i-coord
 * @param j j-coord
 * @return true, if point falls on the edge
 * @return false, if the point doesn't fall on the edge
 */
bool is_edge(int i, int j) {
  if (i == 0 && j >= J1 ||   // A
      i == NX ||             // C
      j == NY ||             // B
      i >= I1 && j == 0 ||   // D
      i == I1 && j <= J1 ||  // E
      i <= I1 && j == J1) {  // F
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Set the boundary conditions for PSI.
 *
 * @param grid the grid we're solving
 * @param Q_in
 */
void PSI_BC(comp_grid& grid, double Q_in) {
  for (int j = J1; j <= NY; ++j) {  // A
    double y = DELTA * j;
    double y_j1 = DELTA * J1;
    double y_ny = DELTA * NY;

    grid[0][j] =
        (Q_in / (2 * MU)) *
        (std::pow(y, 3) / 3 - 0.5 * y * y * (y_j1 + y_ny) + y * y_j1 * y_ny);
  }

  for (int j = 0; j <= NY; ++j) {  // C
    double y = DELTA * j;
    double y_j1 = DELTA * J1;
    double y_ny = DELTA * NY;

    grid[NX][j] =
        (Q_out(Q_in) / (2 * MU)) * (std::pow(y, 3) / 3 - 0.5 * y * y * y_ny) +
        (Q_in * y_j1 * y_j1 * (-y_j1 + 3 * y_ny)) / (12 * MU);
  }

  for (int i = 1; i < NX; ++i) {  // B
    grid[i][NY] = grid[0][NY];
  }

  for (int i = I1; i < NX; ++i) {  // D
    grid[i][0] = grid[0][J1];
  }

  for (int j = 1; j <= J1; ++j) {  // E
    grid[I1][j] = grid[0][J1];
  }

  for (int i = 1; i <= I1; ++i) {  // F
    grid[i][J1] = grid[0][J1];
  }
}

/**
 * @brief Set the boundary conditions for ZETA.
 *
 * @param grid the grid we're solving
 * @param Q_in
 */
void ZETA_BC(comp_grid& grid, double Q_in) {
  for (int j = J1; j <= NY; ++j) {  // A
    double y = DELTA * j;
    double y_j1 = DELTA * J1;
    double y_ny = DELTA * NY;
    grid[0][j] = (Q_in / (2 * MU)) * (2 * y - y_j1 - y_ny);
  }

  for (int j = 0; j <= NY; ++j) {  // C
    double y = DELTA * j;
    double y_ny = DELTA * NY;
    grid[NX][j] = (Q_out(Q_in) / (2 * MU)) * (2 * y - y_ny);
  }

  for (int i = 1; i < NX; ++i) {  // B
    grid[i][NY] = (2 / (DELTA * DELTA)) * (grid[i][NY - 1] - grid[i][NY]);
  }

  for (int i = I1 + 1; i < NX; ++i) {  // D
    grid[i][0] = (2 / (DELTA * DELTA)) * (grid[i][1] - grid[i][0]);
  }

  for (int j = 1; j < J1; ++j) {  // E
    grid[I1][j] = (2 / (DELTA * DELTA)) * (grid[I1 + 1][j] - grid[I1][j]);
  }

  for (int i = 1; i <= I1; ++i) {  // F
    grid[i][J1] = (2 / (DELTA * DELTA)) * (grid[i][J1 + 1] - grid[i][J1]);
  }

  grid[I1][J1] = 0.5 * (grid[I1 - 1][J1] + grid[I1][J1 - 1]);  // E/F apex
}
