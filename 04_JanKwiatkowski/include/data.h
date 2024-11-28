#ifndef DATA_H
#define DATA_H

#include <fstream>
#include <vector>
#include "types.h"

void save_solution(std::ofstream& file, const comp_grid& solution);

void save_iter(std::ofstream& file, const std::vector<double>& iter);

#endif /* DATA_H */
