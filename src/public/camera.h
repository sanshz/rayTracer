#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera
{
private:
  int imageHeight {};
  double pixelSamplesScale {};
  Point3 center {};
  Point3 pixel00Loc {};
  Vec3 pixelDeltaU {};
  Vec3 pixelDeltaV {};

public:
  double aspectRatio {1.0};
  int imageWidth {100};
  int samplesPerPixel {10};

private:
  void initialize()
  {
    imageHeight = static_cast<int>(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    pixelSamplesScale = 1.0 / samplesPerPixel;

    center = Point3 {0.0, 0.0, 0.0};

    auto focalLength {1.0};
    auto viewportHeight {2.0};
    auto viewportWidth {viewportHeight * (static_cast<double>(imageWidth) / imageHeight)};

    auto viewportU {Vec3 {viewportWidth, 0.0, 0.0}};
    auto viewportV {Vec3 {0.0, -viewportHeight, 0.0}};

    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    auto viewportUpperLeft {center - Vec3 {0.0, 0.0, focalLength} - (viewportU / 2) - (viewportV / 2)};
    pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
  }

  Ray getRay(int i, int j) const
  {
    auto offset {sampleSquare()};
    auto pixelSample {
      pixel00Loc
      + ((i + offset.x()) * pixelDeltaU)
      + ((j + offset.y()) * pixelDeltaV)
    };

    auto rayOrigin {center};
    auto rayDirection {pixelSample - rayOrigin};

    return Ray {rayOrigin, rayDirection};
  }

  Vec3 sampleSquare() const
  { return Vec3 {randomDouble() - 0.5, randomDouble() - 0.5, 0.0}; }

  Color rayColor(const Ray& r, const Hittable& world) const
  {
    HitRecord rec {};
    if (world.hit(r, Interval {0.0, infinity}, rec))
    { return 0.5 * (rec.m_normal + Color {1.0, 1.0, 1.0}); }
    
    Vec3 unitDirection {unitVector(r.direction())};
    auto a {0.5 * (unitDirection.y() + 1.0)};
    return ((1.0 - a) * Color {1.0, 1.0, 1.0}) + (a * Color {0.5, 0.7, 1.0});
  }

public:
  void render(const Hittable& world)
  {
    initialize();

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j {0}; j < imageHeight; ++j)
    {
      std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
      for (int i {0}; i < imageWidth; ++i)
      {
        Color pixelColor {0.0, 0.0, 0.0};
        for (int sample {0}; sample < samplesPerPixel; ++sample)
        {
          Ray r {getRay(i, j)};
          pixelColor += rayColor(r, world);
        }
        writeColor(std::cout, pixelSamplesScale * pixelColor);
      }
    }
    std::clog << "\rDone.                     \n";
  }
};

#endif
