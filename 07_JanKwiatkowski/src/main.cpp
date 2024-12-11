#include "navier_stokes.h"

int main() {
  solve(-1000);
  solve(-4000);
  solve(4000);

  return 0;
}