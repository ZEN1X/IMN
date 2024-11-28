//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#ifndef DATA_H
#define DATA_H

#include <fstream>
#include "types.h"

void save_data(std::ofstream& file,
               const vec_step& vec_curr,
               double t,
               double dt);

#endif  // DATA_H
