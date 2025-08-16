#pragma warning (disable : 4514)
#pragma warning (disable : 4820)
#pragma warning (disable : 5045)

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"

int main()
{
  HittableList world {};
  world.add(std::make_shared<Sphere>(Point3 {0.0, 0.0, -1.0}, 0.5));
  world.add(std::make_shared<Sphere>(Point3 {0.0, -100.5, -1.0}, 100));

  Camera cam {};

  cam.m_aspectRatio = (16.0 / 9.0);
  cam.m_imageWidth = 960; // 960x540
  cam.m_samplesPerPixel = 128;
  cam.m_maxDepth = 50;

  cam.render(world);

  return 0;
}
