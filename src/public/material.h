#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material
{
public:
  virtual ~Material() = default;

  virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const
  { return false; }
};

class Lambertian : public Material
{
private:
  Color m_albedo {};

public:
  Lambertian(const Color& albedo)
    : m_albedo {albedo}
  {}

  bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
  {
    auto scatterDirection {rec.m_normal + randomUnitVector()};
    if (scatterDirection.nearZero()) { scatterDirection = rec.m_normal; }

    scattered = Ray {rec.m_p, scatterDirection};
    attenuation = m_albedo;
    return true;
  }
};

class Metal : public Material
{
private:
  Color m_albedo {};
  double m_fuzz {};

public:
  Metal(const Color& albedo, double fuzz)
    : m_albedo {albedo}, m_fuzz {(fuzz < 1 ? fuzz : 1)}
  {}

  bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
  {
    Vec3 reflected {reflect(rIn.direction(), rec.m_normal)};
    reflected = unitVector(reflected) + (m_fuzz * randomUnitVector());
    scattered = Ray {rec.m_p, reflected};
    attenuation = m_albedo;
    return (dot(scattered.direction(), rec.m_normal) > 0);
  }
};

class Dielectric : public Material
{
private:
  double m_refractionIndex {};

private:
  static double reflectance(double cosine, double refractionIndex)
  {
    auto r0 {(1.0 - refractionIndex) / (1.0 + refractionIndex)};
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1.0 - cosine), 5.0);
  }

public:
  Dielectric(double refractionIndex)
    : m_refractionIndex {refractionIndex}
  {}

  bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
  {
    attenuation = Color {1.0, 1.0, 1.0};
    double ri {rec.m_frontFace ? (1.0 / m_refractionIndex) : m_refractionIndex};

    Vec3 unitDirection {unitVector(rIn.direction())};
    double cosTheta {std::fmin(dot(-unitDirection, rec.m_normal), 1.0)};
    double sinTheta {std::sqrt(1.0 - cosTheta * cosTheta)};

    bool cannotRefract {(ri * sinTheta) > 1.0};
    Vec3 direction {};
    if (cannotRefract || (reflectance(cosTheta, ri) > randomDouble()))
    { direction = reflect(unitDirection, rec.m_normal); }
    else { direction = refract(unitDirection, rec.m_normal, ri); }

    scattered = Ray {rec.m_p, direction};
    return true;
  }
};

#endif
