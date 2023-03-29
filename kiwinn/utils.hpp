#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <bits/stdc++.h>
#include <random>
#include <chrono>


double fast_white_noise();

static unsigned int g_seed;
inline void fast_srand(int seed) {
  //Seed the generator
  g_seed = seed;
}
inline int fastrand() {
  //fastrand routine returns one integer, similar output value range as C lib.
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}
inline int fastRandInt(int maxSize) {
  return fastrand() % maxSize;
}
inline int fastRandInt(int a, int b) {
  return(a + fastRandInt(b - a));
}
inline double fastRandDouble() {
  return static_cast<double>(fastrand()) / 0x7FFF;
}
inline double fastRandDouble(double a, double b) {
  return a + (static_cast<double>(fastrand()) / 0x7FFF)*(b-a);
}

#endif