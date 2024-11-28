#include <fstream>
#include <iostream>
#include "data.h"
#include "density.h"
#include "relaxation.h"

int main(void) {
  auto density = calculate_density();

  /*
  GLOBAL RELAX
  */
  std::ofstream S06_sol("S06_sol.csv");
  std::ofstream S10_sol("S10_sol.csv");

  std::ofstream S06_err("S06_err.csv");
  std::ofstream S10_err("S10_err.csv");

  std::ofstream S06_iter("S06_iter.csv");
  std::ofstream S10_iter("S10_iter.csv");

  // solve
  auto relax06 = global_relax(density, 0.6);
  auto relax10 = global_relax(density, 1.0);

  // save solution
  save_solution(S06_sol, relax06.first);
  save_solution(S10_sol, relax10.first);

  // calc error
  auto err06 = calc_error(relax06.first, density);
  auto err10 = calc_error(relax10.first, density);

  // save error
  save_solution(S06_err, err06);
  save_solution(S10_err, err10);

  // save iter
  save_iter(S06_iter, relax06.second);
  save_iter(S10_iter, relax10.second);

  S10_iter.close();
  S06_iter.close();
  S10_err.close();
  S06_err.close();
  S10_sol.close();
  S06_sol.close();

  /*
  LOCAL RELAX
  */
  auto local10 = local_relax(density, 1.0);
  auto local14 = local_relax(density, 1.4);
  auto local18 = local_relax(density, 1.8);
  auto local19 = local_relax(density, 1.9);

  std::ofstream local10_iter("local10_iter.csv");
  std::ofstream local14_iter("local14_iter.csv");
  std::ofstream local18_iter("local18_iter.csv");
  std::ofstream local19_iter("local19_iter.csv");

  save_iter(local10_iter, local10.second);
  save_iter(local14_iter, local14.second);
  save_iter(local18_iter, local18.second);
  save_iter(local19_iter, local19.second);

  local10_iter.close();
  local14_iter.close();
  local18_iter.close();
  local19_iter.close();

  return 0;
}
