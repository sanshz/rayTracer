#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class Material;

class HitRecord
{
public:
  Point3 m_p {};
  Vec3 m_normal {};
  std::shared_ptr<Material> m_mat {};
  double m_t {};
  bool m_frontFace {};
  double m_u, m_v;

  void setFaceNormal(const Ray& r, const Vec3& outwardNormal)
  {
    m_frontFace = (dot(r.direction(), outwardNormal) < 0);
    m_normal = (m_frontFace ? outwardNormal : -outwardNormal);
  }
};

class Hittable
{
public:
  virtual ~Hittable() = default;

  virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;

  virtual AABB boundingBox() const = 0;
};

#endif
