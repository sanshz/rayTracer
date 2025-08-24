#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

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

#endif
