#include "data.h"
#include <iomanip>

void save_solution(std::ofstream& file, const comp_grid& solution) {
  for (std::size_t row = NY; row-- > 0;) {        // row -> j
    for (std::size_t col = 0; col < NX; ++col) {  // col -> i
      file << solution[row][col];
      if (col != NX - 1) {
        file << ',';
      }
    }
    file << '\n';
  }
}

void save_iter(std::ofstream& file, const std::vector<double>& iter) {
  file << std::fixed << std::setprecision(18);

  for (auto val : iter) {
    file << val << '\n';
  }
}
