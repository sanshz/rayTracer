#ifndef AABB_H
#define AABB_H

#include "interval.h"

class AABB
{
public:
  Interval m_x, m_y, m_z;

private:
  void padToMinimums()
  {
    double delta {0.0001};
    if (std::size(m_x) < delta) { m_x = m_x.expand(delta); }
    if (std::size(m_y) < delta) { m_y = m_y.expand(delta); }
    if (std::size(m_z) < delta) { m_z = m_z.expand(delta); }
  }

public:
  AABB() {};

  AABB(const Interval& x, const Interval& y, const Interval& z)
    : m_x {x}, m_y {y}, m_z {z}
  {
    padToMinimums();
  }

  AABB(const Point3& a, const Point3& b)
  {
    m_x = (a[0] <= b[0]) ? Interval {a[0], b[0]} : Interval {b[0], a[0]};
    m_y = (a[1] <= b[1]) ? Interval {a[1], b[1]} : Interval {b[1], a[1]};
    m_z = (a[2] <= b[2]) ? Interval {a[2], b[2]} : Interval {b[2], a[2]};
    padToMinimums();
  }

  AABB(const AABB& box0, const AABB& box1)
  {
    m_x = Interval {box0.m_x, box1.m_x};
    m_y = Interval {box0.m_y, box1.m_y};
    m_z = Interval {box0.m_z, box1.m_z};
  }

  const Interval& axisInterval(int n) const
  {
    if (n == 1) { return m_y; }
    if (n == 2) { return m_z; }
    return m_x;
  }

  bool hit(const Ray& r, Interval rayT) const
  {
    const Point3 rayOrigin {r.origin()};
    const Vec3& rayDirection {r.direction()};

    for (int axis {0}; axis < 3; ++axis)
    {
      const Interval& ax {axisInterval(axis)};
      const double adinv {1.0 / rayDirection[axis]};

      auto t0 {(ax.m_min - rayOrigin[axis]) * adinv};
      auto t1 {(ax.m_max - rayOrigin[axis]) * adinv};

      if (t0 < t1)
      {
        if (t0 > rayT.m_min) { rayT.m_min = t0; }
        if (t1 < rayT.m_max) { rayT.m_max = t1; }
      }
      else
      {

        if (t1 > rayT.m_min) { rayT.m_min = t1; }
        if (t0 < rayT.m_max) { rayT.m_max = t0; }
      }

      if (rayT.m_max <= rayT.m_min) { return false; }
    }

    return true;
  }

  int longestAxis() const
  {
    if (std::size(m_x) > std::size(m_y))
    { return std::size(m_x) > std::size(m_z) ? 0 : 2; }
    else
    { return std::size(m_y) > std::size(m_z) ? 1 : 2; }
  }

  static const AABB empty, universe;
};

const AABB AABB::empty = AABB {Interval::empty, Interval::empty, Interval::empty};
const AABB AABB::universe = AABB {Interval::universe, Interval::universe, Interval::universe};

#endif
