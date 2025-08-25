#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

constexpr double infinity {std::numeric_limits<double>::infinity()};
constexpr double pi {3.1415926535897932385};

inline double degreesToRadians(double degrees)
{ return degrees * (pi / 180.0); }

inline double randomDouble()
{
  static std::uniform_real_distribution<double> distribution {0.0, 1.0};
  static std::mt19937 generator;
  return distribution(generator);
}

inline double randomDouble(double min, double max)
{ return min + (max - min) * randomDouble(); }

inline int randomInt(int min, int max)
{ return static_cast<int>(randomDouble(min, max + 1.0)); }

#include "timer.h"
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
