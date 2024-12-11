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
void PSI_BC(comp_grid& PSI, double Q_in) {
  for (int j = J1; j <= NY; ++j) {  // A
    double y = DELTA * j;
    double y_j1 = DELTA * J1;
    double y_ny = DELTA * NY;

    PSI[0][j] =
        (Q_in / (2 * MU)) *
        (std::pow(y, 3) / 3 - 0.5 * y * y * (y_j1 + y_ny) + y * y_j1 * y_ny);
  }

  for (int j = 0; j <= NY; ++j) {  // C
    double y = DELTA * j;
    double y_j1 = DELTA * J1;
    double y_ny = DELTA * NY;

    PSI[NX][j] =
        (Q_out(Q_in) / (2 * MU)) * (std::pow(y, 3) / 3 - 0.5 * y * y * y_ny) +
        (Q_in * y_j1 * y_j1 * (-y_j1 + 3 * y_ny)) / (12 * MU);
  }

  for (int i = 1; i < NX; ++i) {  // B
    PSI[i][NY] = PSI[0][NY];
  }

  for (int i = I1; i < NX; ++i) {  // D
    PSI[i][0] = PSI[0][J1];
  }

  for (int j = 1; j <= J1; ++j) {  // E
    PSI[I1][j] = PSI[0][J1];
  }

  for (int i = 1; i <= I1; ++i) {  // F
    PSI[i][J1] = PSI[0][J1];
  }
}

/**
 * @brief Set the boundary conditions for ZETA.
 *
 * @param grid the grid we're solving
 * @param Q_in
 */
void ZETA_BC(comp_grid& ZETA, double Q_in) {
  for (int j = J1; j <= NY; ++j) {  // A
    double y = DELTA * j;
    double y_j1 = DELTA * J1;
    double y_ny = DELTA * NY;
    ZETA[0][j] = (Q_in / (2 * MU)) * (2 * y - y_j1 - y_ny);
  }

  for (int j = 0; j <= NY; ++j) {  // C
    double y = DELTA * j;
    double y_ny = DELTA * NY;
    ZETA[NX][j] = (Q_out(Q_in) / (2 * MU)) * (2 * y - y_ny);
  }

  for (int i = 1; i < NX; ++i) {  // B
    ZETA[i][NY] = (2 / (DELTA * DELTA)) * (ZETA[i][NY - 1] - ZETA[i][NY]);
  }

  for (int i = I1 + 1; i < NX; ++i) {  // D
    ZETA[i][0] = (2 / (DELTA * DELTA)) * (ZETA[i][1] - ZETA[i][0]);
  }

  for (int j = 1; j < J1; ++j) {  // E
    ZETA[I1][j] = (2 / (DELTA * DELTA)) * (ZETA[I1 + 1][j] - ZETA[I1][j]);
  }

  for (int i = 1; i <= I1; ++i) {  // F
    ZETA[i][J1] = (2 / (DELTA * DELTA)) * (ZETA[i][J1 + 1] - ZETA[i][J1]);
  }

  ZETA[I1][J1] = 0.5 * (ZETA[I1 - 1][J1] + ZETA[I1][J1 - 1]);  // E/F apex
}

double calculate_GAMMA(const comp_grid& PSI, const comp_grid& ZETA) {
  double gamma = 0.0;

  for (int i = 1; i < NX; ++i) {
    gamma += PSI[i + 1][J2] + PSI[i - 1][J2] + PSI[i][J2 + 1] + PSI[i][J2 - 1] -
             4 * PSI[i][J2] - DELTA * DELTA * ZETA[i][J2];
  }

  return gamma;
}
