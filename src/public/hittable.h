#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class Material;

class HitRecord
{
public:
  Point3 m_p;
  Vec3 m_normal;
  std::shared_ptr<Material> m_mat;
  double m_t;
  bool m_frontFace;
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

class Translate : public Hittable
{
private:
  std::shared_ptr<Hittable> m_object;
  Vec3 m_offset;
  AABB m_bBox;

public:
  Translate(std::shared_ptr<Hittable> object, const Vec3& offset)
    : m_object {object}, m_offset {offset}
  { m_bBox = m_object->boundingBox() + m_offset; }

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    Ray offsetR {r.origin() - m_offset, r.direction(), r.time()};

    if (!m_object->hit(offsetR, rayT, rec)) { return false; }

    rec.m_p += m_offset;

    return true;
  }

  AABB boundingBox() const override { return m_bBox; }
};

class RotateY : public Hittable
{
private:
  std::shared_ptr<Hittable> m_object;
  double m_sinTheta;
  double m_cosTheta;
  AABB m_bBox;

public:
  RotateY(std::shared_ptr<Hittable> object, double angle)
    : m_object {object}
  {
    auto radians {degreesToRadians(angle)};
    m_sinTheta = std::sin(radians);
    m_cosTheta = std::cos(radians);
    m_bBox = m_object->boundingBox();

    Point3 min {infinity, infinity, infinity};
    Point3 max {-infinity, -infinity, -infinity};

    for (int i {0}; i < 2; ++i)
    {
      for (int j {0}; j < 2; ++j)
      {
        for (int k {0}; k < 2; ++k)
        {
          auto x {i * m_bBox.m_x.m_max + (1 - i) * m_bBox.m_x.m_min};
          auto y {j * m_bBox.m_y.m_max + (1 - j) * m_bBox.m_y.m_min};
          auto z {k * m_bBox.m_z.m_max + (1 - k) * m_bBox.m_z.m_min};

          auto newX {m_cosTheta * x + m_sinTheta * z};
          auto newZ {-m_sinTheta * x + m_cosTheta * z};

          Vec3 tester {newX, y, newZ};

          for (int c {0}; c < 3; ++c)
          {
            min[c] = std::fmin(min[c], tester[c]);
            max[c] = std::fmax(max[c], tester[c]);
          }
        }
      }
    }
    m_bBox = AABB {min, max};
  }

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    auto origin {
      Point3 {
        (m_cosTheta * r.origin().x()) - (m_sinTheta * r.origin().z()),
        r.origin().y(),
        (m_sinTheta * r.origin().x()) + (m_cosTheta * r.origin().z())
      }
    };

    auto direction {
      Vec3 {
        (m_cosTheta * r.direction().x()) - (m_sinTheta * r.direction().z()),
        r.direction().y(),
        (m_sinTheta * r.direction().x()) + (m_cosTheta * r.direction().z())
      }
    };

    Ray rotatedR {origin, direction, r.time()};

    if (!m_object->hit(rotatedR, rayT, rec)) { return false; }

    rec.m_p = Point3 {
      (m_cosTheta * rec.m_p.x()) + (m_sinTheta * rec.m_p.z()),
      rec.m_p.y(),
      (-m_sinTheta * rec.m_p.x()) + (m_cosTheta * rec.m_p.z())
    };

    rec.m_normal = Vec3 {
      (m_cosTheta * rec.m_normal.x()) + (m_sinTheta * rec.m_normal.z()),
      rec.m_normal.y(),
      (-m_sinTheta * rec.m_normal.x()) + (m_cosTheta * rec.m_normal.z())
    };

    return true;
  }

  AABB boundingBox() const override { return m_bBox; }
};

#endif
