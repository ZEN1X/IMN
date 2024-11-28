#include "density.h"
#include <cmath>

double dens1(double x, double y) {
  return +1 * std::exp(-std::pow((x - 0.35 * XMAX) / (SIGMAX), 2) - std::pow((y - 0.5 * YMAX) / (SIGMAY), 2));
}

double dens2(double x, double y) {
  return -1 * std::exp(-std::pow((x - 0.65 * XMAX) / (SIGMAX), 2) - std::pow((y - 0.5 * YMAX) / (SIGMAY), 2));
}

comp_grid calculate_density() {
  comp_grid density_grid;

  for (std::size_t j = 0; j < density_grid.size(); ++j) {       // iterating rows -> j
    for (std::size_t i = 0; i < density_grid[0].size(); ++i) {  // iterating columns -> i
      // i and j swapped, refer to comment in types.h
      double x = DELTA * i;
      double y = DELTA * j;
      density_grid[j][i] = dens1(x, y) + dens2(x, y);
    }
  }

  return density_grid;
}
