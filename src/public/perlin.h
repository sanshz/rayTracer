#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class Perlin
{
private:
  static const int m_pointCount {256};
  Vec3 m_randVec[m_pointCount];
  int m_permX[m_pointCount];
  int m_permY[m_pointCount];
  int m_permZ[m_pointCount];

private:
  static void perlinGeneratePerm(int* p)
  {
    for (int i {0}; i < m_pointCount; ++i)
    { p[i] = i; }

    permute(p, m_pointCount);
  }

  static void permute(int* p, int n)
  {
    for (int i {n - 1}; i > 0; --i)
    {
      int target {randomInt(0, i)};
      int tmp {p[i]};
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static double perlinInterp(const Vec3 c[2][2][2], double u, double v, double w)
  {
    auto uu {u * u * (3 - 2 * u)};
    auto vv {v * v * (3 - 2 * v)};
    auto ww {w * w * (3 - 2 * w)};
    auto accum {0.0};

    for (int i {0}; i < 2; ++i)
    {
      for (int j {0}; j < 2; ++j)
      {
        for (int k {0}; k < 2; ++k)
        {
          Vec3 weightV {u - i, v - j, w - k};
          accum += (
            (i * uu + (1 - i) * (1 - uu)) *
            (j * vv + (1 - j) * (1 - vv)) *
            (k * ww + (1 - k) * (1 - ww)) *
            dot(c[i][j][k], weightV)
          );
        }
      }
    }
    return accum;
  }

public:
  Perlin()
  {
    for (int i {0}; i < m_pointCount; ++i)
    { m_randVec[i] = unitVector(Vec3::random(-1, 1)); }

    perlinGeneratePerm(m_permX);
    perlinGeneratePerm(m_permY);
    perlinGeneratePerm(m_permZ);
  }

  double noise(const Point3& p) const
  {
    auto u {p.x() - std::floor(p.x())};
    auto v {p.y() - std::floor(p.y())};
    auto w {p.z() - std::floor(p.z())};

    auto i {static_cast<int>(std::floor(p.x()))};
    auto j {static_cast<int>(std::floor(p.y()))};
    auto k {static_cast<int>(std::floor(p.z()))};
    Vec3 c[2][2][2];

    for (int di {0}; di < 2; ++di)
    {
      for (int dj {0}; dj < 2; ++dj)
      {
        for (int dk {0}; dk < 2; ++dk)
        {
          c[di][dj][dk] = m_randVec[
            m_permX[(i + di) & 255] ^
            m_permY[(j + dj) & 255] ^
            m_permZ[(k + dk) & 255]
          ];
        }
      }
    }
    return perlinInterp(c, u, v, w);
  }

  double turb(const Point3& p, int depth) const
  {
    auto accum {0.0};
    auto tempP {p};
    auto weight {1.0};

    for (int i {0}; i < depth; ++i)
    {
      accum += (weight * noise(tempP));
      weight *= 0.5;
      tempP *= 2;
    }
    return std::fabs(accum);
  }
};

#endif
