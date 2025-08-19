#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>

class HittableList : public Hittable
{
public:
  std::vector<std::shared_ptr<Hittable>> m_objects {};

public:
  HittableList() {};

  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  void clear() { m_objects.clear(); }

  void add(std::shared_ptr<Hittable> object)
  { m_objects.push_back(object); }

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    HitRecord tempRec {};
    bool hitAnything {false};
    auto closestSoFar {rayT.m_max};

    for (const auto& object : m_objects)
    {
      if (object->hit(r, Interval {rayT.m_min, closestSoFar}, tempRec))
      {
        hitAnything = true;
        closestSoFar = tempRec.m_t;
        rec = tempRec;
      }
    }
    return hitAnything;
  }
};

#endif
