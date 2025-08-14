#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using Color = Vec3;

void writeColor(std::ostream& out, const Color& pixelColor)
{
  auto r {pixelColor.x()};
  auto g {pixelColor.y()};
  auto b {pixelColor.z()};

  // [0, 1] to [0, 255]
  int rByte {static_cast<int>(255.999 * r)};
  int gByte {static_cast<int>(255.999 * g)};
  int bByte {static_cast<int>(255.999 * b)};

  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif
