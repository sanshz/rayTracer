#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class Interval
{
public:
  double m_min, m_max;
  
public:
  Interval()
  : m_min {+infinity}, m_max {-infinity}
  {}
  
  Interval(double min, double max)
  : m_min {min}, m_max {max}
  {}

  Interval(const Interval& a, const Interval& b)
  {
    m_min = (a.m_min <= b.m_min) ? a.m_min : b.m_min;
    m_max = (a.m_max >= b.m_max) ? a.m_max : b.m_max;
  }
  
  double size() const { return m_max - m_min; }
  
  bool contains(double x) const { return ((m_min <= x) && (x <= m_max)); }
  
  bool surrounds(double x) const { return ((m_min < x) && (x < m_max)); }

  double clamp(double x) const
  {
    if (x < m_min) { return m_min; }
    if (x > m_max) { return m_max; }
    return x;
  }

  Interval expand(double delta) const
  {
    auto padding {delta / 2};
    return Interval {m_min - padding, m_max - padding};
  }

  static const Interval empty, universe;
};

const Interval Interval::empty {Interval {+infinity, -infinity}};
const Interval Interval::universe {Interval {-infinity, +infinity}};

#endif
