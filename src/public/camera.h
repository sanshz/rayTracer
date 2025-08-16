#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera
{
private:
  int m_imageHeight {};
  double m_pixelSamplesScale {};
  Point3 m_center {};
  Point3 m_pixel00Loc {};
  Vec3 m_pixelDeltaU {};
  Vec3 m_pixelDeltaV {};

public:
  double m_aspectRatio {1.0};
  int m_imageWidth {100};
  int m_samplesPerPixel {10};
  int m_maxDepth {10};

private:
  void initialize()
  {
    m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

    m_pixelSamplesScale = 1.0 / m_samplesPerPixel;

    m_center = Point3 {0.0, 0.0, 0.0};

    auto focalLength {1.0};
    auto viewportHeight {2.0};
    auto viewportWidth {viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight)};

    auto viewportU {Vec3 {viewportWidth, 0.0, 0.0}};
    auto viewportV {Vec3 {0.0, -viewportHeight, 0.0}};

    m_pixelDeltaU = viewportU / m_imageWidth;
    m_pixelDeltaV = viewportV / m_imageHeight;

    auto viewportUpperLeft {m_center - Vec3 {0.0, 0.0, focalLength} - (viewportU / 2) - (viewportV / 2)};
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
  }

  Ray getRay(int i, int j) const
  {
    auto offset {sampleSquare()};
    auto pixelSample {
      m_pixel00Loc
      + ((i + offset.x()) * m_pixelDeltaU)
      + ((j + offset.y()) * m_pixelDeltaV)
    };

    auto rayOrigin {m_center};
    auto rayDirection {pixelSample - rayOrigin};

    return Ray {rayOrigin, rayDirection};
  }

  Vec3 sampleSquare() const
  { return Vec3 {randomDouble() - 0.5, randomDouble() - 0.5, 0.0}; }

  Color rayColor(const Ray& r, int depth, const Hittable& world) const
  {
    if (depth <= 0) { return Color {0.0, 0.0, 0.0}; }

    HitRecord rec {};
    if (world.hit(r, Interval {0.001, infinity}, rec))
    {
      Vec3 direction {rec.m_normal + randomUnitVector()};
      return 0.5 * rayColor(Ray {rec.m_p, direction}, (depth - 1), world);
    }

    Vec3 unitDirection {unitVector(r.direction())};
    auto a {0.5 * (unitDirection.y() + 1.0)};
    return ((1.0 - a) * Color {1.0, 1.0, 1.0}) + (a * Color {0.5, 0.7, 1.0});
  }

public:
  void render(const Hittable& world)
  {
    initialize();

    std::cout << "P3\n" << m_imageWidth << ' ' << m_imageHeight << "\n255\n";

    for (int j {0}; j < m_imageHeight; ++j)
    {
      std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << ' ' << std::flush;
      for (int i {0}; i < m_imageWidth; ++i)
      {
        Color pixelColor {0.0, 0.0, 0.0};
        for (int sample {0}; sample < m_samplesPerPixel; ++sample)
        {
          Ray r {getRay(i, j)};
          pixelColor += rayColor(r, m_maxDepth, world);
        }
        writeColor(std::cout, m_pixelSamplesScale * pixelColor);
      }
    }
    std::clog << "\rDone.                     \n";
  }
};

#endif
