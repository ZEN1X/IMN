#include "convection-diffusion.h"

int main() {
  save_v_field();

  solve(0.0);
  solve(0.1);

  return 0;
}
