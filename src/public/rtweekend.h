#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <cmath>
#include <iostream>
#include <memory>

constexpr double infinity {std::numeric_limits<double>::infinity()};
constexpr double pi {3.1415926535897932385};

inline double degreesToRadians(double degrees)
{ return degrees * (pi / 180.0); }

#include "timer.h"
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
