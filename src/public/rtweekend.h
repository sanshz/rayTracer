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

inline double randomDouble(double min = 0.0, double max = 1.0)
{
  static std::uniform_real_distribution<double> distribution {min, max};
  static std::mt19937 generator {};
  return distribution(generator);
}

#include "timer.h"
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
