#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable
{
private:
  Ray m_center {};
  double m_radius {};
  std::shared_ptr<Material> m_mat {};
  AABB m_bBox;

private:
  static void getSphereUV(const Point3& p, double& u, double& v)
  {
    auto theta {std::acos(-p.y())};
    auto phi {std::atan2(-p.z(), p.x()) + pi};

    u = phi / (2.0 * pi);
    v = theta / pi;
  }

public:
  // Stationary Sphere
  Sphere(const Point3& staticCenter, double radius, std::shared_ptr<Material> mat)
    : m_center {staticCenter, Vec3 {0.0, 0.0, 0.0}}, m_radius {std::fmax(0.0, radius)}, m_mat {mat}
  {
    auto rVec {Vec3 {radius, radius, radius}};
    m_bBox = AABB {staticCenter - rVec, staticCenter + rVec};
  }

  // Moving Sphere
  Sphere(const Point3& center1, const Point3& center2, double radius, std::shared_ptr<Material> mat)
    : m_center {center1, center2 - center1}, m_radius {std::fmax(0.0, radius)}, m_mat {mat}
  {
    auto rVec {Vec3 {radius, radius, radius}};
    AABB box1 {m_center.at(0) - rVec, m_center.at(0) + rVec};
    AABB box2 {m_center.at(1) - rVec, m_center.at(1) + rVec};
    m_bBox = AABB {box1, box2};
  }

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
    getSphereUV(outwardNormal, rec.m_u, rec.m_v);
    rec.m_mat = m_mat;

    return true;
  }

  AABB boundingBox() const override { return m_bBox; }
};

#endif
