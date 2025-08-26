#ifndef RAY_H
#define RAY_H

#include "rtweekend.h"

class Ray
{
private:
  Point3 m_origin;
  Vec3 m_direction;
  double m_time;

public:
  Ray() {};

  Ray(const Point3& origin, const Vec3& direction, double time)
    : m_origin {origin}, m_direction {direction}, m_time {time}
  {}

  Ray(const Point3& origin, const Vec3& direction)
    : m_origin {origin}, m_direction {direction}, m_time {0.0}
  {}

  const Point3& origin() const { return m_origin; }
  const Vec3& direction() const { return m_direction; }

  double time() const { return m_time; }

  Point3 at(double t) const { return (m_origin + (t * m_direction)); }
};

#endif
