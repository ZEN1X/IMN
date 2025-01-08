#include "crank_nicolson.h"

int main() {
  Crank_Nicolson cn{};
  cn.solve();

  return 0;
}