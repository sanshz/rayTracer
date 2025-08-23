#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "perlin.h"
#include "rtwStbImage.h"

class Texture
{
public:
  virtual ~Texture() = default;

  virtual Color value(double u, double v, const Point3& p) const = 0;
};


class SolidColor : public Texture
{
private:
  Color m_albedo;  

public:
  SolidColor(const Color& albedo)
    : m_albedo {albedo}
  {}

  SolidColor(double red, double green, double blue)
   : SolidColor {Color {red, green, blue}}
  {}

  Color value(double u, double v, const Point3& p) const override { return m_albedo; }
};


class CheckerTexture : public Texture
{
private:
  double m_invScale;
  std::shared_ptr<Texture> m_even;
  std::shared_ptr<Texture> m_odd;

public:
  CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
    : m_invScale {1.0 / scale}, m_even {even}, m_odd {odd}
  {}

  CheckerTexture(double scale, const Color& c1, const Color& c2)
    : CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2))
  {}

  Color value(double u, double v, const Point3& p) const override
  {
    auto xInteger {static_cast<int>(std::floor(m_invScale * p.x()))};
    auto yInteger {static_cast<int>(std::floor(m_invScale * p.y()))};
    auto zInteger {static_cast<int>(std::floor(m_invScale * p.z()))};

    bool isEven {(xInteger + yInteger + zInteger) % 2 == 0};

    return (isEven ? m_even->value(u, v, p) : m_odd->value(u, v, p));
  }
};

class ImageTexture : public Texture
{
private:
  RTWImage m_image;

public:
  ImageTexture(const char* filename)
    : m_image {filename}
  {}

  Color value(double u, double v, const Point3& p) const override
  {
    if (m_image.height() <= 0) { return Color {0.0, 1.0, 1.0}; }

    u = (Interval {0, 1}).clamp(u);
    v = 1.0 - (Interval {0, 1}).clamp(v);

    auto i {static_cast<int>(u * m_image.width())};
    auto j {static_cast<int>(v * m_image.height())};
    auto pixel {m_image.pixelData(i, j)};

    auto colorScale {1.0 / 255.0};
    return Color {colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]};
  }
};

class NoiseTexture : public Texture
{
private:
  Perlin m_noise;
  double m_scale;

public:
  NoiseTexture(double scale)
    : m_scale {scale}
  {}

  Color value(double u, double v, const Point3& p) const override
  { return ((Color {0.5, 0.5, 0.5}) * (1 + std::sin(m_scale * p.z() + 10 * m_noise.turb(p, 7)))); }
  // { return ((Color {1.0, 1.0, 1.0}) * m_noise.turb(p, 7)); }
  // { return ((Color {1.0, 1.0, 1.0}) * 0.5 * (1.0 + m_noise.noise(m_scale * p))); }
};

#endif
