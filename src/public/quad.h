#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "hittableList.h"

class Quad : public Hittable
{
private:
  Point3 m_Q;
  Vec3 m_u, m_v;
  Vec3 m_w;
  std::shared_ptr<Material> m_mat;
  AABB m_bBox;
  Vec3 m_normal;
  double m_D;

public:
  Quad(const Point3& Q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> mat)
    : m_Q {Q}, m_u {u}, m_v {v}, m_mat {mat}
  {
    auto n {cross(m_u, m_v)};
    m_normal = unitVector(n);
    m_D = dot(m_normal, m_Q);
    m_w = n / dot(n, n);
    setBoundingBox();
  }

  virtual void setBoundingBox()
  {
    auto bBoxDiagonal1 {AABB {m_Q, m_Q + m_u + m_v}};
    auto bBoxDiagonal2 {AABB {m_Q + m_u, m_Q + m_v}};
    m_bBox = AABB {bBoxDiagonal1, bBoxDiagonal2};
  }

  AABB boundingBox() const override { return m_bBox; }

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  { 
    auto denom {dot(m_normal, r.direction())};

    if (std::fabs(denom) < 1e-8) { return false; }

    auto t {(m_D - dot(m_normal, r.origin())) / denom};
    if (!rayT.contains(t)) { return false; }

    auto intersection {r.at(t)};
    Vec3 planarHitPtVector {intersection - m_Q};
    auto alpha {dot(m_w, cross(planarHitPtVector, m_v))};
    auto beta {dot(m_w, cross(m_u, planarHitPtVector))};

    if (!isInterior(alpha, beta, rec)) { return false; }

    rec.m_t = t;
    rec.m_p = intersection;
    rec.m_mat = m_mat;
    rec.setFaceNormal(r, m_normal);

    return true;
  }

  virtual bool isInterior(double a, double b, HitRecord& rec) const
  {
    Interval unitInterval {Interval {0.0, 1.0}};

    if (!unitInterval.contains(a) || !unitInterval.contains(b)) { return false; }

    rec.m_u = a;
    rec.m_v = b;
    return true;
  }
};

inline std::shared_ptr<HittableList> box(const Point3& a, const Point3& b, std::shared_ptr<Material> mat)
{
  auto sides {std::make_shared<HittableList>()};

  auto min {Point3 {std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z())}};
  auto max {Point3 {std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z())}};

  auto dx {Vec3 {max.x() - min.x(), 0.0, 0.0}};
  auto dy {Vec3 {0.0, max.y() - min.y(), 0.0}};
  auto dz {Vec3 {0.0, 0.0, max.z() - min.z()}};

  sides->add(std::make_shared<Quad>(Point3 {min.x(), min.y(), max.z()}, dx, dy, mat));
  sides->add(std::make_shared<Quad>(Point3 {max.x(), min.y(), max.z()}, -dz, dy, mat));
  sides->add(std::make_shared<Quad>(Point3 {max.x(), min.y(), min.z()}, -dx, dy, mat));
  sides->add(std::make_shared<Quad>(Point3 {min.x(), min.y(), min.z()}, dz, dy, mat));
  sides->add(std::make_shared<Quad>(Point3 {min.x(), max.y(), max.z()}, dx, -dz, mat));
  sides->add(std::make_shared<Quad>(Point3 {min.x(), min.y(), min.z()}, dx, dz, mat));

  return sides;
}

#endif
