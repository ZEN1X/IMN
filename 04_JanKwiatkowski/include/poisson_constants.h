#ifndef POISSON_CONSTANTS_H
#define POISSON_CONSTANTS_H

constexpr double EPSILON = 1.0;
constexpr double DELTA = 0.1;
constexpr int NX = 150 + 1; // no of nodes, not max x
constexpr int NY = 100 + 1;
constexpr double V1 = 10.0;
constexpr double V2 = 0.0;

constexpr double XMAX = DELTA * (NX-1); // actual max x
constexpr double YMAX = DELTA * (NY-1);

constexpr double SIGMAX = 0.1 * XMAX;
constexpr double SIGMAY = 0.1 * YMAX;

#endif /* POISSON_CONSTANTS_H */
