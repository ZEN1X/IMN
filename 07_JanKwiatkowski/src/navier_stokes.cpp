#include "navier_stokes.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Calculate Q_out based on Q_in.
 *
 * @param Q_in
 * @return double, calculated Q_out
 */
double Q_out(double Q_in) {
  return Q_in * std::pow((NY - J1) / (double)NY, 3);
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
void ZETA_BC(comp_grid& ZETA, const comp_grid& PSI, double Q_in) {
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
    ZETA[i][NY] = (2 / (DELTA * DELTA)) * (PSI[i][NY - 1] - PSI[i][NY]);
  }

  for (int i = I1 + 1; i < NX; ++i) {  // D
    ZETA[i][0] = (2 / (DELTA * DELTA)) * (PSI[i][1] - PSI[i][0]);
  }

  for (int j = 1; j < J1; ++j) {  // E
    ZETA[I1][j] = (2 / (DELTA * DELTA)) * (PSI[I1 + 1][j] - PSI[I1][j]);
  }

  for (int i = 1; i <= I1; ++i) {  // F
    ZETA[i][J1] = (2 / (DELTA * DELTA)) * (PSI[i][J1 + 1] - PSI[i][J1]);
  }

  ZETA[I1][J1] = 0.5 * (ZETA[I1 - 1][J1] + ZETA[I1][J1 - 1]);  // E/F apex
}

/**
 * @brief Calculate the GAMMA error value.
 *
 * @param PSI PSI grid
 * @param ZETA ZETA grid
 * @return double, calculated GAMMA error
 */
double calculate_GAMMA(const comp_grid& PSI, const comp_grid& ZETA) {
  double gamma = 0.0;

  for (int i = 1; i < NX; ++i) {
    gamma += PSI[i + 1][J2] + PSI[i - 1][J2] + PSI[i][J2 + 1] + PSI[i][J2 - 1] -
             4 * PSI[i][J2] - DELTA * DELTA * ZETA[i][J2];
  }

  return gamma;
}

/**
 * @brief Solve the Navier-Stokes equation using relaxation.
 *
 * @param Q_in
 */
void solve(double Q_in) {
  // declare and zero-init grids
  comp_grid PSI{}, ZETA{}, U{}, V{};

  // initialize PSI
  PSI_BC(PSI, Q_in);

  // relax
  for (int it = 1; it <= IT_MAX; ++it) {
    double OMEGA = it < 2000 ? 0 : 1;

    for (int i = 1; i < NX; ++i) {
      for (int j = 1; j < NY; ++j) {
        if (!is_edge(i, j)) {
          PSI[i][j] = 0.25 * (PSI[i + 1][j] + PSI[i - 1][j] + PSI[i][j + 1] +
                              PSI[i][j - 1] - DELTA * DELTA * ZETA[i][j]);

          ZETA[i][j] = 0.25 * (ZETA[i + 1][j] + ZETA[i - 1][j] +
                               ZETA[i][j + 1] + ZETA[i][j - 1]) -
                       OMEGA * (RHO / (16 * MU)) *
                           ((PSI[i][j + 1] - PSI[i][j - 1]) *
                                (ZETA[i + 1][j] - ZETA[i - 1][j]) -
                            (PSI[i + 1][j] - PSI[i - 1][j]) *
                                (ZETA[i][j + 1] - ZETA[i][j - 1]));

          // central derivative in y-direction
          U[i][j] = (PSI[i][j + 1] - PSI[i][j - 1]) / (2 * DELTA);
          // central derivative in x-direction
          V[i][j] = -(PSI[i + 1][j] - PSI[i - 1][j]) / (2 * DELTA);
        }
      }
    }

    // modify ZETA
    ZETA_BC(ZETA, PSI, Q_in);

    // error-control
    double gamma = calculate_GAMMA(PSI, ZETA);
    if (it % 100 == 0) {
      std::cout << "Iter: " << it << ", GAMMA: " << gamma << '\n';
    }
  }

  // save output
  std::string filename = "wyn";
  if (Q_in == 4000) {
    filename += "_Q4000";
  } else if (Q_in == -4000) {
    filename += "_Qm4000";
  }
  filename += ".dat";

  std::ofstream file(filename);

  // dont include edges, otherwise the plot breaks
  for (int i = 1; i < NX; ++i) {
    for (int j = 1; j < NY; ++j) {
      file << i * DELTA << ' ' << j * DELTA << ' ' << PSI[i][j] << ' '
           << ZETA[i][j] << ' ' << U[i][j] << ' ' << V[i][j] << '\n';
    }

    file << '\n';
  }
}