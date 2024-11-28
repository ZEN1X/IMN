//
// Created by Jan Kwiatkowski on 27/11/2024.
//

#include "multigrid_relax.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <string>

double VB1(double y) {
    return std::sin(M_PI * y / YMAX);
}

double VB2(double x) {
    return -std::sin(2 * M_PI * x / XMAX);
}

double VB3(double y) {
    return std::sin(M_PI * y / YMAX);
}

double VB4(double x) {
    return std::sin(2 * M_PI * x / XMAX);
}

comp_grid initialize_grid() {
    comp_grid grid{}; // value-init -> 0.0 for double

    // left edge -> VB1 & right edge -> VB3
    for (int j = 0; j <= NY; ++j) {
        // <= to iterate 0 ... NY (inclusive, cause size is +1)
        double y = j * DELTA;
        grid[0][j] = VB1(y);
        grid[NX][j] = VB3(y);
    }

    // top edge -> CB2 & bottom edge -> VB4
    for (int i = 0; i <= NX; ++i) {
        double x = i * DELTA;
        grid[i][NY] = VB2(x);
        grid[i][0] = VB4(x);
    }

    return grid;
}

double calc_S(const comp_grid &V, const int k) {
    double S{};

    for (int i = 0; i <= NX - k; i += k) {
        for (int j = 0; j <= NY - k; j += k) {
            const double first_term = std::pow(k * DELTA, 2) / 2.0;
            const double common_denominator = 2.0 * k * DELTA;

            const double second_term = (V[i + k][j] - V[i][j] + V[i + k][j + k] - V[i][j + k]) / common_denominator;
            const double third_term = (V[i][j + k] - V[i][j] + V[i + k][j + k] - V[i + k][j]) / common_denominator;

            S += first_term * (std::pow(second_term, 2) + std::pow(third_term, 2));
        }
    }

    return S;
}

void make_denser(comp_grid &V, const int k) {
    const int k2 = k / 2;

    for (int i = 0; i <= NX - k; i += k) {
        for (int j = 0; j <= NY - k; j += k) {
            V[i + k2][j + k2] = 0.25 * (V[i][j] + V[i + k][j] + V[i][j + k] + V[i + k][j + k]);

            if (i != NX - k) {
                V[i + k][j + k2] = 0.5 * (V[i + k][j] + V[i + k][j + k]);
            }

            if (j != NY - k) {
                V[i + k2][j + k] = 0.5 * (V[i][j + k] + V[i + k][j + k]);
            }

            if (j != 0) {
                V[i + k2][j] = 0.5 * (V[i][j] + V[i + k][j]);
            }

            if (i != 0) {
                V[i][j + k2] = 0.5 * (V[i][j] + V[i][j + k]);
            }
        }
    }
}

// vector because I don't want to make it a template function and write everything in a header, just to use std::array...
void multigrid_relax(const std::vector<int> &Ks) {
    comp_grid V{initialize_grid()};

    int iter{};

    // iterate over all Ks
    for (auto k: Ks) {
        double S_previous{};
        double S{};

        // for each k, open a new output file
        std::ofstream iter_file("Sk" + std::to_string(k) + ".dat");
        std::ofstream v_file("Vk" + std::to_string(k) + ".dat");

        double stop;

        do {
            iter++;

            // step over the interior (< NX) and relax with the "k" step
            for (int i = k; i < NX; i += k) {
                for (int j = k; j < NY; j += k) {
                    V[i][j] = 0.25 * (V[i + k][j] + V[i - k][j] + V[i][j + k] + V[i][j - k]);
                }
            }

            // ok, now calculate the S integral
            S = calc_S(V, k);

            // save S(it)
            iter_file << iter << '\t' << S << '\n';

            // save V
            for (int i = 0; i <= NX; i += k) {
                for (int j = 0; j <= NY; j += k) {
                    v_file << i * DELTA << '\t' << j * DELTA << '\t' << V[i][j] << '\n';
                }
                v_file << '\n';
            }

            // calculate the condition for stopping the loop
            stop = std::abs((S - S_previous) / S_previous);

            // now change the Ss around
            S_previous = S;
        } while (stop > TOL);

        // now we've finished relaxing for that k, make the grid denser
        if (k > 1) {
            make_denser(V, k);
        }
    }
}
