#ifndef RK4_H
#define RK4_H

#include <array>
#include <functional>

template <typename T, std::size_t N, typename F>
void rk4_vec(T t, T dt, std::array<T, N>& s, F&& f) {
  static std::array<T, N> k1{}, k2{}, k3{}, k4{}, w{};  // re-use the arrays on subsequent calls
                                                        // w is a "workspace" / helper array

  // k1
  w = s;
  f(t, w, k1);

  // k2
  for (std::size_t i = 0; i < N; i++) {
    w[i] = s[i] + dt / 2.0 * k1[i];
  }
  f(t + dt / 2.0, w, k2);

  // k3
  for (std::size_t i = 0; i < N; i++) {
    w[i] = s[i] + dt / 2.0 * k2[i];
  }
  f(t + dt / 2.0, w, k3);

  // k4
  for (std::size_t i = 0; i < N; i++) {
    w[i] = s[i] + dt * k3[i];
  }
  f(t + dt, w, k4);

  // next / calculated step
  for (std::size_t i = 0; i < N; i++) {
    s[i] += dt / 6.0 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
  }
}

#endif /* RK4_H */
