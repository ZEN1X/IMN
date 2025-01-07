#ifndef CRANK_NICOLSON_H
#define CRANK_NICOLSON_H

// constants
constexpr int NX = 40;
constexpr int NY = 40;
constexpr int N = (NX + 1) * (NY + 1);

constexpr double DELTA = 1.0;
constexpr double DELTA_T = 1.0;

constexpr double TA = 40;
constexpr double TB = 0;
constexpr double TC = 30;
constexpr double TD = 0;

constexpr double KB = 0.1;
constexpr double KD = 0.6;

constexpr int IT_MAX = 2'000;

#endif /* CRANK_NICOLSON_H */
