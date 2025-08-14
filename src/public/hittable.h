#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class HitRecord
{
public:
  Point3 m_p {};
  Vec3 m_normal {};
  double m_t {};
  bool m_frontFace {};

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
};

#endif
