//
// Created by Jan Kwiatkowski on 27/11/2024.
//

#ifndef MULTIGRID_RELAX_H
#define MULTIGRID_RELAX_H

#include "types.h"
#include <vector>

inline double VB1(double y);

inline double VB2(double x);

inline double VB3(double y);

inline double VB4(double x);

comp_grid initialize_grid();

double calc_S(const comp_grid &V, int k);

void make_denser(comp_grid &V, int k);

void multigrid_relax(const std::vector<int> &Ks);

#endif //MULTIGRID_RELAX_H
