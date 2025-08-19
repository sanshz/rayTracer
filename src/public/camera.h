#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class Camera
{
private:
  int m_imageHeight {};
  double m_pixelSamplesScale {};
  Point3 m_center {};
  Point3 m_pixel00Loc {};
  Vec3 m_pixelDeltaU {};
  Vec3 m_pixelDeltaV {};
  Vec3 m_u {}, m_v {}, m_w {};
  Vec3 m_defocusDiskU {};
  Vec3 m_defocusDiskV {};

public:
  double m_aspectRatio {1.0};
  int m_imageWidth {100};
  int m_samplesPerPixel {10};
  int m_maxDepth {10};

  double m_vFov {90};
  Point3 m_lookFrom {0.0, 0.0, 0.0};
  Point3 m_lookAt {0.0, 0.0, -1.0};
  Vec3 m_vUp {0.0, 1.0, 0.0};

  double m_defocusAngle {0.0};
  double m_focusDist {10.0};

private:
  void initialize()
  {
    m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

    m_pixelSamplesScale = 1.0 / m_samplesPerPixel;

    m_center = m_lookFrom;

    auto theta {degreesToRadians(m_vFov)};
    auto h {std::tan(theta / 2)};
    auto viewportHeight {2.0 * h * m_focusDist};
    auto viewportWidth {viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight)};

    m_w = unitVector(m_lookFrom - m_lookAt);
    m_u = unitVector(cross(m_vUp, m_w));
    m_v = cross(m_w, m_u);

    Vec3 viewportU {viewportWidth * m_u};
    Vec3 viewportV {viewportHeight * -m_v};

    m_pixelDeltaU = viewportU / m_imageWidth;
    m_pixelDeltaV = viewportV / m_imageHeight;

    auto viewportUpperLeft {m_center - (m_focusDist * m_w) - (viewportU / 2) - (viewportV / 2)};
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    auto defocusRadius {m_focusDist * std::tan(degreesToRadians(m_defocusAngle / 2))};
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;
  }

  Ray getRay(int i, int j) const
  {
    auto offset {sampleSquare()};
    auto pixelSample {
      m_pixel00Loc
      + ((i + offset.x()) * m_pixelDeltaU)
      + ((j + offset.y()) * m_pixelDeltaV)
    };

    auto rayOrigin {(m_defocusAngle <= 0) ? m_center : defocusDiskSample()};
    auto rayDirection {pixelSample - rayOrigin};

    return Ray {rayOrigin, rayDirection};
  }

  Vec3 sampleSquare() const
  { return Vec3 {randomDouble() - 0.5, randomDouble() - 0.5, 0.0}; }

  Point3 defocusDiskSample() const
  {
    auto p {randomInUnitDisk()};
    return m_center + (p[0] * m_defocusDiskU) + (p[1] * m_defocusDiskV);
  }

  Color rayColor(const Ray& r, int depth, const Hittable& world) const
  {
    if (depth <= 0) { return Color {0.0, 0.0, 0.0}; }

    HitRecord rec {};
    if (world.hit(r, Interval {0.001, infinity}, rec))
    {
      Ray scattered {};
      Color attenuation {};
      if (rec.m_mat->scatter(r, rec, attenuation, scattered))
      { return attenuation * rayColor(scattered, depth - 1, world); }

      return Color {0.0, 0.0, 0.0};
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

    Timer tS {};
    for (int j {0}; j < m_imageHeight; ++j)
    {
      if (j % 36 == 0)
      { std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << ' ' << std::flush; }
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
    std::clog << "Time elapsed: " <<  tS.elapsed() << '\n';
  }
};

#endif
