#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
private:
  Point3 m_origin {};
  Vec3 m_direction {};

public:
  Ray() = default;

  Ray(const Point3& origin, const Vec3& direction)
    : m_origin {origin}, m_direction {direction}
  {}

  const Point3& origin() const { return m_origin; }
  const Vec3& direction() const { return m_direction; }

  Point3 at(double t) const { return (m_origin + (t * m_direction)); }
};

#endif
