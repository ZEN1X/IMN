#ifndef RELAXATION_H
#define RELAXATION_H

#include "types.h"
#include <vector>

comp_grid generate_init_cond(void);

double calc_S(const comp_grid& V, const comp_grid& density);

std::pair<comp_grid, std::vector<double>> global_relax(const comp_grid& density, double omega_g, double tolerance = 1e-8);

std::pair<comp_grid, std::vector<double>> local_relax(const comp_grid& density, double omega_l, double tolerance = 1e-8);

comp_grid calc_error(const comp_grid& V, const comp_grid& density);

#endif /* RELAXATION_H */
