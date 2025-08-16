#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include <vector>

class Vec3
{
public:
  std::vector<double> m_e {0, 0, 0};

public:
  Vec3() = default;

  Vec3(double e0, double e1, double e2)
    : m_e {e0, e1, e2}
  {}

  double x() const { return m_e[0]; }
  double y() const { return m_e[1]; }
  double z() const { return m_e[2]; }

  Vec3 operator-() const { return Vec3 {-m_e[0], -m_e[1], -m_e[2]}; }
  double operator[](int i) const { return m_e[static_cast<std::size_t>(i)]; }
  double& operator[](int i) { return m_e[static_cast<std::size_t>(i)]; }

  Vec3& operator+=(const Vec3& v)
  {
    m_e[0] += v.m_e[0];
    m_e[1] += v.m_e[1];
    m_e[2] += v.m_e[2];
    return *this;
  }

  Vec3& operator*=(double t)
  {
    m_e[0] *= t;
    m_e[1] *= t;
    m_e[2] *= t;
    return *this;
  }

  Vec3& operator/=(double t) { return ((*this) *= (1 / t)); }

  double length() const { return std::sqrt(lengthSquared());}

  double lengthSquared() const
  { return ((m_e[0] * m_e[0]) + (m_e[1] * m_e[1]) + (m_e[2] * m_e[2])); }

  static Vec3 random()
  { return Vec3 {randomDouble(), randomDouble(), randomDouble()}; }

  static Vec3 random(double min, double max)
  { return Vec3 {randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)}; }
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{ return out << v.m_e[0] << ' ' << v.m_e[1] << ' ' << v.m_e[2]; }

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{ return Vec3 {u.m_e[0] + v.m_e[0], u.m_e[1] + v.m_e[1], u.m_e[2] + v.m_e[2]}; }

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{ return Vec3 {u.m_e[0] - v.m_e[0], u.m_e[1] - v.m_e[1], u.m_e[2] - v.m_e[2]}; }

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{ return Vec3 {u.m_e[0] * v.m_e[0], u.m_e[1] * v.m_e[1], u.m_e[2] * v.m_e[2]}; }

inline Vec3 operator*(double t, const Vec3& v)
{ return Vec3 {t * v.m_e[0], t * v.m_e[1], t * v.m_e[2]}; }

inline Vec3 operator*(const Vec3& v, double t)
{ return t * v; }

inline Vec3 operator/(const Vec3& v, double t)
{ return ((1 / t) * v); }

inline double dot(const Vec3& u, const Vec3& v)
{ return ((u.m_e[0] * v.m_e[0]) + (u.m_e[1] * v.m_e[1]) + (u.m_e[2] * v.m_e[2])); }

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
  return Vec3
  {
    (u.m_e[1] * v.m_e[2]) - (u.m_e[2] * v.m_e[1]),
    (u.m_e[2] * v.m_e[0]) - (u.m_e[0] * v.m_e[2]),
    (u.m_e[0] * v.m_e[1]) - (u.m_e[1] * v.m_e[0])
  };
}

inline Vec3 unitVector(const Vec3& v)
{ return (v / v.length()); }

inline Vec3 randomUnitVector()
{
  while (true)
  {
    auto p {Vec3::random(-1.0, 1.0)};
    auto lensq {p.lengthSquared()};
    if ((1e-160 < lensq) && (lensq <= 1))
    { return p / sqrt(lensq); }
  }
}

inline Vec3 randomOnHemisphere(const Vec3& normal)
{
  Vec3 onUnitSphere {randomUnitVector()};
  if (dot(onUnitSphere, normal) > 0.0) { return onUnitSphere; }
  else { return -onUnitSphere; }
}

#endif
