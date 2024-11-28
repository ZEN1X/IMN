//
// Created by Jan Kwiatkowski on 12/11/2024.
//

#include "data.h"

void save_data(std::ofstream& file,
               const vec_step& vec_curr,
               double t,
               double dt) {
  file << t << ',' << vec_curr.first << ',' << vec_curr.second << ',' << dt << '\n';
}