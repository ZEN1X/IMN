#include <fstream>
#include <iomanip>
#include "implicit_rk2.h"
#include "trapeze.h"

const double BETA = 0.001;
const int N = 500;
const double GAMMA = 0.1;

double SIS_model(double t, double u) {
  return (BETA * N - GAMMA) * u - BETA * u * u;
}

double SIS_derivative(double t, double u) {
  return BETA * N - GAMMA - 2 * BETA * u;
}

int main(int argc, char const* argv[]) {
  // Trapeze method using Picard's iteration
  auto picard_results = trapeze(SIS_model, SIS_derivative, false);

  std::ofstream out{"picard.csv"};
  out << std::fixed << std::setprecision(12);
  for (auto value : picard_results) {
    out << value.first << ',' << value.second << '\n';
  }
  out.close();

  // Trapeze method using Newton's iteration
  auto newton_results = trapeze(SIS_model, SIS_derivative, true);

  out.open("newton.csv");
  for (auto value : newton_results) {
    out << value.first << ',' << value.second << '\n';
  }
  out.close();

  // Implicit RK2 method
  auto rk2_results = implicit_rk2(SIS_model, SIS_derivative);

  out.open("rk2.csv");
  for (auto value : rk2_results) {
    out << value.first << ',' << value.second << '\n';
  }
  out.close();

  return 0;
}
