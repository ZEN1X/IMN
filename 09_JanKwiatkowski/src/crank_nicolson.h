#ifndef CRANK_NICOLSON_H
#define CRANK_NICOLSON_H

#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_vector_double.h>

class Crank_Nicolson {
 private:
  // constants
  static constexpr int NX = 40;
  static constexpr int NY = 40;
  static constexpr int N = (NX + 1) * (NY + 1);

  static constexpr double DELTA = 1.0;
  static constexpr double DELTA_T = 1.0;

  static constexpr double TA = 40;
  static constexpr double TB = 0;
  static constexpr double TC = 30;
  static constexpr double TD = 0;

  static constexpr double KB = 0.1;
  static constexpr double KD = 0.6;

  static constexpr int IT_MAX = 2'000;

  // matrices, vectors etc. -> solution representation
  gsl_matrix *A, *B;
  gsl_vector *c, *T;

  // helper function for flattening the representation
  int get_l(int i, int j);

  // initialize matrices, vectors and calc initial conditions
  void cn_init();

  // init inside only
  void cn_init_inside();

  // init edges only
  void cn_init_edges();

  // initial conditions
  void cn_init_ic();

  // save to file
  void save(int it);

 public:
  Crank_Nicolson();
  ~Crank_Nicolson();

  void solve();
};

#endif /* CRANK_NICOLSON_H */
