#ifndef TYPES_H
#define TYPES_H

#include <array>
#include "poisson_constants.h"

// x, y, but in multi-dim array the indices get swapped during declaration
// also, i refers to the horizontal index, while j to the vertical
// thus one more swap... and we get this
using comp_grid = std::array<std::array<double, NX>, NY>;

#endif /* TYPES_H */
