#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable
{
private:
  Ray m_center {};
  double m_radius {};
  std::shared_ptr<Material> m_mat {};

public:
  Sphere(const Point3& staticCenter, double radius, std::shared_ptr<Material> mat)
    : m_center {staticCenter, Vec3 {0.0, 0.0, 0.0}}, m_radius {std::fmax(0.0, radius)}, m_mat {mat}
  {}

  Sphere(const Point3& center1, const Point3& center2, double radius, std::shared_ptr<Material> mat)
    : m_center {center1, center2 - center1}, m_radius {std::fmax(0.0, radius)}, m_mat {mat}
  {}

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    Point3 currentCenter {m_center.at(r.time())};
    Vec3 oc {currentCenter - r.origin()};
    auto a {r.direction().lengthSquared()};
    auto h {dot(r.direction(), oc)};
    auto c {oc.lengthSquared() - (m_radius * m_radius)};

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
    Vec3 outwardNormal {(rec.m_p - currentCenter) / m_radius};
    rec.setFaceNormal(r, outwardNormal);
    rec.m_mat = m_mat;

    return true;
  }
};

#endif
