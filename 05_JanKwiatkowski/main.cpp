#include <iostream>
#include "multigrid_relax.h"

int main() {
    const std::vector<int> Ks{16, 8, 4, 2, 1};
    multigrid_relax(Ks);

    return 0;
}
