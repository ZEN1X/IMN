#include <cmath>
#include <fstream>
#include "algebraic_solver.h"
#include "constants.h"

void print_test_a_b(const init_vectors &data) {
    auto &[a, ja, ia, b] = data;
    const int N = static_cast<int>(b.size());
    const int nx = static_cast<int>(std::sqrt(N)) - 1;

    std::ofstream file("test_matrix_ab.dat");

    file << "# l" << '\t' << "i_l" << '\t' << "j_l" << '\t' << "b[l]\n";
    for (int l = 0; l < N; ++l) {
        file << l << '\t' << get_i(l, nx) * DELTA << '\t' << get_j(l, nx) * DELTA << '\t' << b[l] << '\n';
    }

    file << "\n# k" << '\t' << "a[k]\n";
    for (decltype(a)::size_type k = 0; k < a.size(); ++k) {
        file << k << '\t' << a[k] << '\n';
    }
}

int main() {
    /*
     * 2.3
     */
    std::array<double, 4> Vs{10, -10, 10, -10};
    init_vectors vec{initialize_vectors(4, 4, Vs, false, 1, 1)};
    print_test_a_b(vec);

    /*
     * 2.5
     */

    // a
    vec = initialize_vectors(50, 50, Vs, false, 1, 1);
    solve(vec, "v50.dat");

    // b
    vec = initialize_vectors(100, 100, Vs, false, 1, 1);
    solve(vec, "v100.dat");

    // c
    vec = initialize_vectors(200, 200, Vs, false, 1, 1);
    solve(vec, "v200.dat");

    /*
     * 2.6
     */
    Vs = {0, 0, 0, 0};

    // a
    vec = initialize_vectors(100, 100, Vs, true, 1, 1);
    solve(vec, "g_e11.dat");

    // b
    vec = initialize_vectors(100, 100, Vs, true, 1, 2);
    solve(vec, "g_e12.dat");

    // c
    vec = initialize_vectors(100, 100, Vs, true, 1, 10);
    solve(vec, "g_e110.dat");

    return 0;
}
