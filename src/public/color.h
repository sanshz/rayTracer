#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using Color = Vec3;

inline double linearToGamma(double linearComponent)
{
  if (linearComponent > 0)
  { return std::sqrt(linearComponent); }
  return 0;
}

void writeColor(std::ostream& out, const Color& pixelColor)
{
  auto r {linearToGamma(pixelColor.x())};
  auto g {linearToGamma(pixelColor.y())};
  auto b {linearToGamma(pixelColor.z())};

  // [0, 1] to [0, 255]
  static const Interval intensity {0.000, 0.999};
  int rByte {static_cast<int>(256 * intensity.clamp(r))};
  int gByte {static_cast<int>(256 * intensity.clamp(g))};
  int bByte {static_cast<int>(256 * intensity.clamp(b))};

  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif
