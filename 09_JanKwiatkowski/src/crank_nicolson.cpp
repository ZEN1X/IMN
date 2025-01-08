#include "crank_nicolson.h"
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector_double.h>
#include <fstream>
#include <indicators/indicators.hpp>
#include <initializer_list>
#include <string>
#include "gsl/gsl_blas.h"

int Crank_Nicolson::get_l(int i, int j) {
  return i + j * (NX + 1);
}

void Crank_Nicolson::cn_init() {
  // inside
  cn_init_inside();

  // edges
  cn_init_edges();

  // T vector
  cn_init_ic();
}

void Crank_Nicolson::cn_init_inside() {
  // common factor in all expressions
  double a = DELTA_T / (2.0 * DELTA * DELTA);

  // don't repeat the same code for A and B
  auto set_neighbors = [this](gsl_matrix* m, int i, int j, double center_val,
                              double neighbor_val) {
    int l = get_l(i, j);

    gsl_matrix_set(m, l, l - NX - 1, neighbor_val);  // top
    gsl_matrix_set(m, l, l - 1, neighbor_val);       // left
    gsl_matrix_set(m, l, l + 1, neighbor_val);       // right
    gsl_matrix_set(m, l, l + NX + 1, neighbor_val);  // bottom
    gsl_matrix_set(m, l, l, center_val);             // center
  };

  // initialize
  for (int i = 1; i < NX; ++i) {
    for (int j = 1; j < NY; ++j) {
      set_neighbors(A, i, j, -4 * a - 1, a);
      set_neighbors(B, i, j, 4 * a - 1, -a);
    }
  }
}

void Crank_Nicolson::cn_init_edges() {
  // left & right -> Dirichlet
  for (int i : {0, NX}) {
    for (int j = 0; j <= NY; ++j) {
      int l = get_l(i, j);
      gsl_matrix_set(A, l, l, 1);
      gsl_matrix_set(B, l, l, 1);
      gsl_vector_set(c, l, 0);
    }
  }

  // top von Neumann
  for (int i = 1; i < NX; ++i) {
    int l = get_l(i, NY);
    double a = 1.0 / (KB * DELTA);  // common factor

    gsl_matrix_set(A, l, l - NX - 1, -a);
    gsl_matrix_set(A, l, l, 1 + a);

    gsl_vector_set(c, l, TB);

    for (int ast = 0; ast < N; ++ast) {
      gsl_matrix_set(B, l, ast, 0);
    }
  }

  // bottom von Neumann
  for (int i = 1; i < NX; ++i) {
    int l = get_l(i, 0);
    double a = 1.0 / (KD * DELTA);  // common factor

    gsl_matrix_set(A, l, l, 1 + a);
    gsl_matrix_set(A, l, l + NX + 1, -a);

    gsl_vector_set(c, l, TD);

    for (int ast = 0; ast < N; ++ast) {
      gsl_matrix_set(B, l, ast, 0);
    }
  }
}

void Crank_Nicolson::cn_init_ic() {
  gsl_vector_set_all(T, 0);

  for (int j = 0; j <= NY; ++j) {
    gsl_vector_set(T, get_l(0, j), TA);
    gsl_vector_set(T, get_l(NX, j), TC);
  }
}

void Crank_Nicolson::save(int it) {
  std::string filename{std::to_string(it) + ".dat"};
  std::ofstream out{filename};

  auto d2T = [](gsl_vector* T, int l) {
    return ((gsl_vector_get(T, l + 1) - 2 * gsl_vector_get(T, l) +
             gsl_vector_get(T, l - 1)) /
            std::pow(DELTA, 2)) +
           ((gsl_vector_get(T, l + NX + 1) - 2 * gsl_vector_get(T, l) +
             gsl_vector_get(T, l - NX - 1)) /
            std::pow(DELTA, 2));
  };

  for (int i = 1; i < NX; ++i) {
    for (int j = 1; j < NY; ++j) {
      int l = get_l(i, j);
      double laplacian = d2T(T, l);

      out << i << '\t' << j << '\t' << gsl_vector_get(T, l) << '\t' << laplacian
          << '\n';
    }
    out << '\n';
  }
}

Crank_Nicolson::Crank_Nicolson()
    : A{gsl_matrix_calloc(N, N)},
      B{gsl_matrix_calloc(N, N)},
      c{gsl_vector_calloc(N)},
      T{gsl_vector_calloc(N)} {
  cn_init();
}

Crank_Nicolson::~Crank_Nicolson() {
  gsl_matrix_free(A);
  gsl_matrix_free(B);
  gsl_vector_free(c);
  gsl_vector_free(T);
}

void Crank_Nicolson::solve() {
  gsl_vector* d{gsl_vector_calloc(N)};
  gsl_permutation* p{gsl_permutation_calloc(N)};
  int signum;

  gsl_linalg_LU_decomp(A, p, &signum);

  // progress indicator
  indicators::show_console_cursor(false);

  using namespace indicators;
  indicators::BlockProgressBar bar{
      option::BarWidth{80}, option::ForegroundColor{Color::white},
      option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
      option::MaxProgress{IT_MAX + 1}};

  for (int it = 0; it <= IT_MAX; ++it) {
    gsl_blas_dgemv(CblasNoTrans, 1.0, B, T, 0.0, d);
    gsl_blas_daxpy(1.0, c, d);
    gsl_linalg_LU_solve(A, p, d, T);

    switch (it) {
      case 100:
      case 200:
      case 500:
      case 1000:
      case 2000:
        save(it);
    }

    bar.set_option(
        option::PostfixText{std::to_string(it) + "/" + std::to_string(IT_MAX)});
    bar.tick();
  }

  bar.mark_as_completed();
  indicators::show_console_cursor(true);

  gsl_permutation_free(p);
  gsl_vector_free(d);
}