#pragma warning (disable : 4514)
#pragma warning (disable : 5045)

#include <iostream>
#include "color.h"
#include "ray.h"
#include "vec3.h"

namespace Image
{
  constexpr auto aspectRatio {16.0 / 9.0};
  constexpr int width {540};

  int height {std::max(static_cast<int>(width / aspectRatio), 1)};
}

namespace View
{
  constexpr auto focalLength {1.0};
  constexpr auto height {2.0};
  auto width {height * (static_cast<double>(Image::width) / Image::height)};
  auto cameraCenter { Point3 {0.0, 0.0, 0.0} };
  
  auto u { Vec3 {width, 0.0, 0.0} };
  auto v { Vec3 {0.0, -height, 0.0} };

  auto upperLeft {cameraCenter - Vec3 {0.0, 0.0, focalLength} - (u / 2) - (v / 2)};
}

namespace Pixel
{
  auto deltaU {View::u / Image::width};
  auto deltaV {View::v / Image::height};

  auto loc00 {View::upperLeft + 0.5 * (deltaU + deltaV)};
}

bool hitSphere(const Point3& center, double radius, const Ray& r)
{
  Vec3 oc {center - r.origin()};
  auto a {dot(r.direction(), r.direction())};
  auto b {-2.0 * dot(r.direction(), oc)};
  auto c {dot(oc, oc) - (radius * radius)};
  auto discriminant {(b * b) - (4 * a * c)};
  return (discriminant >= 0);
}

Color rayColor(const Ray& r)
{
  if (hitSphere(Point3 {0.0, 0.0, -1.0}, 0.5, r)) { return Color {1.0, 0.0, 0.0}; }

  Vec3 unitDirection {unitVector(r.direction())};
  auto a {0.5 * (unitDirection.y() + 1.0)};
  return ((1.0 - a) * Color {1.0, 1.0, 1.0}) + (a * Color {0.5, 0.7, 1.0});
}

int main()
{
  std::cout << "P3\n" << Image::width << ' ' << Image::height << "\n255\n";  

  for (int j {0}; j < Image::height; ++j)
  {
    std::clog << "\rScanlines remaining: " << (Image::width - j) << ' ' << std::flush;
    for (int i {0}; i < Image::width; ++i)
    {
      auto pixelCenter {Pixel::loc00 + (i * Pixel::deltaU) + (j * Pixel::deltaV)};
      auto rayDirection {pixelCenter - View::cameraCenter};
      Ray r {View::cameraCenter, rayDirection};

      Color pixelColor {rayColor(r)};
      writeColor(std::cout, pixelColor);
    }
  }

  std::clog << "\rDone. \t\n";

  return 0;
}
