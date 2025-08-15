#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class Interval
{
public:
  double m_min {};
  double m_max {};
  
  Interval()
  : m_min {+infinity}, m_max {-infinity}
  {}
  
  Interval(double min, double max)
  : m_min {min}, m_max {max}
  {}
  
  double size() const { return m_max - m_min; }
  
  bool contains(double x) const { return ((m_min <= x) && (x <= m_max)); }
  
  bool surrounds(double x) const { return ((m_min < x) && (x < m_max)); }

  double clamp(double x) const
  {
    if (x < m_min) { return m_min; }
    if (x > m_max) { return m_max; }
    return x;
  }

  static const Interval empty, universe;
};

const Interval Interval::empty {Interval {+infinity, -infinity}};
const Interval Interval::universe {Interval {-infinity, +infinity}};

#endif
