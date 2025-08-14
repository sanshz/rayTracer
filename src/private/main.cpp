#pragma warning (disable : 4514)
#pragma warning (disable : 4820)
#pragma warning (disable : 5045)

#include "rtweekend.h"

#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"

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

Color rayColor(const Ray& r, const Hittable& world)
{
  HitRecord rec {};
  if (world.hit(r, Interval {0.0, infinity}, rec))
  { return 0.5 * (rec.m_normal + Color {1.0, 1.0, 1.0}); }

  Vec3 unitDirection {unitVector(r.direction())};
  auto a {0.5 * (unitDirection.y() + 1.0)};
  return ((1.0 - a) * Color {1.0, 1.0, 1.0}) + (a * Color {0.5, 0.7, 1.0});
}

int main()
{
  HittableList world {};
  world.add(std::make_shared<Sphere>(Point3 {0.0, 0.0, -1.0}, 0.5));
  world.add(std::make_shared<Sphere>(Point3 {0.0, -100.5, -1.0}, 100));

  std::cout << "P3\n" << Image::width << ' ' << Image::height << "\n255\n";  

  Timer t {};
  for (int j {0}; j < Image::height; ++j)
  {
    std::clog << "\rScanlines remaining: " << (Image::height - j) << ' ' << std::flush;
    for (int i {0}; i < Image::width; ++i)
    {
      auto pixelCenter {Pixel::loc00 + (i * Pixel::deltaU) + (j * Pixel::deltaV)};
      auto rayDirection {pixelCenter - View::cameraCenter};
      Ray r {View::cameraCenter, rayDirection};

      Color pixelColor {rayColor(r, world)};
      writeColor(std::cout, pixelColor);
    }
  }

  std::clog << "Time elapsed: " << t.elapsed() << " seconds\n";
  std::clog << "\nDone.       \n";

  return 0;
}
