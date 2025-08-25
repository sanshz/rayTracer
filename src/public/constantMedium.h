#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "hittable.h"
#include "material.h"
#include "texture.h"

class ConstantMedium : public Hittable
{
private:
  std::shared_ptr<Hittable> m_boundary;
  double m_negInvDensity;
  std::shared_ptr<Material> m_phaseFunction;

public:
  ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> tex)
    : m_boundary {boundary}, m_negInvDensity {-1 / density}, m_phaseFunction {std::make_shared<Isotropic>(tex)}
  {}

  ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& albedo)
    : m_boundary {boundary}, m_negInvDensity {-1 / density}, m_phaseFunction {std::make_shared<Isotropic>(albedo)}
  {}

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    HitRecord rec1, rec2;

    if (!m_boundary->hit(r, Interval::universe, rec1)) { return false; }
    if (!m_boundary->hit(r, Interval {rec1.m_t + 0.0001, infinity}, rec2)) { return false; }

    if (rec1.m_t < rayT.m_min) { rec1.m_t = rayT.m_min; }
    if (rec2.m_t > rayT.m_max) { rec2.m_t = rayT.m_max; }

    if (rec1.m_t >= rec2.m_t) { return false; }

    if (rec1.m_t < 0) { rec1.m_t = 0; }

    auto rayLength {r.direction().length()};
    auto distanceInsideBoundary {(rec2.m_t - rec1.m_t) * rayLength};
    auto hitDistance {m_negInvDensity * std::log(randomDouble())};

    if (hitDistance > distanceInsideBoundary) { return false; }

    rec.m_t = rec1.m_t + hitDistance / rayLength;
    rec.m_p = r.at(rec.m_t);

    rec.m_normal = Vec3 {1.0, 0.0, 0.0};
    rec.m_frontFace = true;
    rec.m_mat = m_phaseFunction;

    return true;
  }

  AABB boundingBox() const override { return m_boundary->boundingBox(); }
};

#endif
