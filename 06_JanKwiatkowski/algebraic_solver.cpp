//
// Created by Jan Kwiatkowski on 01/12/2024.
//

#include "algebraic_solver.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include "constants.h"
#include "mgmres.hpp"


double rho(const double x, const double y, const double xmax, const double ymax, const double sigma) {
    return std::exp(-std::pow((x - 0.25 * xmax) / sigma, 2) - std::pow((y - 0.5 * ymax) / sigma, 2)) +
           -std::exp(-std::pow((x - 0.75 * xmax) / sigma, 2) - std::pow((y - 0.5 * ymax) / sigma, 2));
}

double epsilon(const int l, const int nx, const double epsilon1, const double epsilon2) {
    double eps;

    if (const int i = get_i(l, nx); i <= nx / 2) {
        eps = epsilon1;
    } else {
        eps = epsilon2;
    }

    return eps;
}

init_vectors initialize_vectors(const int nx, const int ny, const std::array<double, 4> &Vs, const bool nonzero_rho,
                                const double epsilon1,
                                const double epsilon2) {
    // constants
    const int N = (nx + 1) * (ny + 1);
    const double xmax = DELTA * nx;
    const double ymax = DELTA * ny;
    const double sigma = xmax / 10.0;

    // vectors for initialization
    // using vectors as I don't want to define the function inside the header (as a template...)
    std::vector<double> a(5 * N);
    std::vector<int> ja(5 * N);
    std::vector ia(N + 1, -1);
    std::vector<double> b(N);

    // begin initialization algo
    int k = -1;
    for (int l = 0; l < N; ++l) {
        bool edge = false; // false if inside, true if on the edge
        double ve = 0.0;

        const int i = get_i(l, nx);
        const int j = get_j(l, nx);

        if (i == 0) {
            // left edge
            edge = true;
            ve = Vs[0];
        }

        if (j == ny) {
            // top edge
            edge = true;
            ve = Vs[1];
        }

        if (i == nx) {
            // right edge
            edge = true;
            ve = Vs[2];
        }

        if (j == 0) {
            // bottom edge
            edge = true;
            ve = Vs[3];
        }

        // initialize vector b
        if (nonzero_rho) {
            const double x = i * DELTA;
            const double y = j * DELTA;
            b[l] = -rho(x, y, xmax, ymax, sigma);
        } else if (edge) {
            b[l] = ve;
        }

        // initialize A
        ia[l] = -1;

        // leftmost diagonal
        if (l - nx - 1 >= 0 && edge == false) {
            ++k;
            if (ia[l] < 0) {
                ia[l] = k;
            }
            a[k] = epsilon(l, nx, epsilon1, epsilon2) / (DELTA * DELTA);
            ja[k] = l - nx - 1;
        }

        // subdiagonal
        if (l - 1 >= 0 && edge == false) {
            ++k;
            if (ia[l] < 0) {
                ia[l] = k;
            }
            a[k] = epsilon(l, nx, epsilon1, epsilon2) / (DELTA * DELTA);
            ja[k] = l - 1;
        }

        // main diagonal
        ++k;
        if (ia[l] < 0) {
            ia[l] = k;
        }
        if (edge == false) {
            a[k] = -(2 * epsilon(l, nx, epsilon1, epsilon2) +
                     epsilon(l + 1, nx, epsilon1, epsilon2) +
                     epsilon(l + nx + 1, nx, epsilon1, epsilon2)) / (DELTA * DELTA);
        } else {
            a[k] = 1;
        }
        ja[k] = l;

        // superdiagonal
        if (l < N && edge == false) {
            ++k;
            a[k] = epsilon(l + 1, nx, epsilon1, epsilon2) / (DELTA * DELTA);
            ja[k] = l + 1;
        }

        // rightmost diagonal
        if (l < N - nx - 1 && edge == false) {
            ++k;
            a[k] = epsilon(l + nx + 1, nx, epsilon1, epsilon2) / (DELTA * DELTA);
            ja[k] = l + nx + 1;
        }
    }

    // number of non-zero entries
    const int nz_num = k + 1;
    ia[N] = nz_num;

    return {a, ja, ia, b};
}

void solve(init_vectors &data, const std::string &filename) {
    auto &[a, ja, ia, b] = data;

    const int N = static_cast<int>(b.size());
    const auto nz_num = ia[N];

    std::vector<double> V(N);

    pmgmres_ilu_cr(static_cast<int>(N), nz_num, ia.data(), ja.data(), a.data(), V.data(), b.data(), 500, 500, 1e-8,
                   1e-8);

    // save to file
    std::ofstream file(filename);
    file << std::fixed << std::setprecision(10);

    // well, since nx == ny, then we can calculate nx like:
    const int nx = static_cast<int>(std::sqrt(N)) - 1; // static_cast to remove clang-tidy warning

    for (int l = 0; l < N; ++l) {
        file << get_i(l, nx) * DELTA << '\t' << get_j(l, nx) * DELTA << '\t' << V[l] << '\n';
        if (l + 1 < N && get_j(l + 1, nx) != get_j(l, nx)) {
            file << '\n';
        }
    }
}
