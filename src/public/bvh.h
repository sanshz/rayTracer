#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittableList.h"

#include <algorithm>

class BVHNode : public Hittable
{
private:
  std::shared_ptr<Hittable> m_left;
  std::shared_ptr<Hittable> m_right;
  AABB m_bBox;

private:
  static bool boxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex)
  {
    auto aAxisInterval {a->boundingBox().axisInterval(axisIndex)};
    auto bAxisInterval {b->boundingBox().axisInterval(axisIndex)};
    return aAxisInterval.m_min < bAxisInterval.m_min;
  }

  static bool boxXcompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
  { return boxCompare(a, b, 0); }

  static bool boxYcompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
  { return boxCompare(a, b, 1); }

  static bool boxZcompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
  { return boxCompare(a, b, 2); }

public:
  BVHNode(HittableList list)
    : BVHNode {list.m_objects, 0, std::size(list.m_objects)}
  {}

  BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end)
  {
    m_bBox = AABB::empty;
    for (std::size_t objectIndex {start}; objectIndex < end; ++objectIndex)
    { m_bBox = AABB {m_bBox, objects[objectIndex]->boundingBox()}; }

    int axis {m_bBox.longestAxis()};

    auto comparator {(axis == 0) ? boxXcompare : (axis == 1) ? boxYcompare : boxZcompare};

    std::size_t objectSpan {end - start};

    if (objectSpan == 1) { m_left = m_right = objects[start]; }
    else if (objectSpan == 2)
    {
      m_left = objects[start];
      m_right = objects[start + 1];
    }
    else 
    { 
      std::sort(std::begin(objects) + static_cast<int>(start), std::begin(objects) + static_cast<int>(end), comparator);

      auto mid {start + (objectSpan / 2)};
      m_left = std::make_shared<BVHNode>(objects, start, mid);
      m_right = std::make_shared<BVHNode>(objects, mid, end);
    }
  }

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    if (!m_bBox.hit(r, rayT)) { return false; }

    bool hitLeft {m_left->hit(r, rayT, rec)};
    bool hitRight {m_right->hit(r, Interval {rayT.m_min, hitLeft ? rec.m_t : rayT.m_max}, rec)};

    return (hitLeft || hitRight);
  }

  AABB boundingBox() const override { return m_bBox; }
};

#endif
