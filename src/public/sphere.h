#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable
{
private:
  Point3 m_center {};
  double m_radius {};

public:
  Sphere(const Point3& center, double radius)
    : m_center {center}, m_radius {std::fmax(0.0, radius)}
  {}

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    Vec3 oc {m_center - r.origin()};
    auto a {r.direction().length_squared()};
    auto h {dot(r.direction(), oc)};
    auto c {oc.length_squared() - (m_radius * m_radius)};

    auto discriminant {(h * h) - (a * c)};
    if (discriminant < 0) { return false; }

    auto sqrtD {std::sqrt(discriminant)};

    auto root {(h - sqrtD) / a};
    if (!rayT.surrounds(root))
    {
      root = (h + sqrtD) / a;
      if (!rayT.surrounds(root)) { return false; }
    }

    rec.m_t = root;
    rec.m_p = r.at(rec.m_t);
    Vec3 outwardNormal {(rec.m_p - m_center) / m_radius};
    rec.setFaceNormal(r, outwardNormal);

    return true;
  }
};

#endif
