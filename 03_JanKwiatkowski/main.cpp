#include <array>
#include <iostream>
#include <sstream>
#include "solver.h"
#include "time_control.h"

int main(void) {
  std::array<double, 2> TOL = {1e-2, 1e-5};

  std::string filename{"rk2_2.csv"};
  time_control(rk2, filename, TOL[0]);

  filename = "rk2_5.csv";
  time_control(rk2, filename, TOL[1]);

  filename = "trapeze_2.csv";
  time_control(trapeze, filename, TOL[0]);

  filename = "trapeze_5.csv";
  time_control(trapeze, filename, TOL[1]);

  return 0;
}
