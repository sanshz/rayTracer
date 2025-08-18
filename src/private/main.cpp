#pragma warning (disable : 4514)
#pragma warning (disable : 4820)
#pragma warning (disable : 5045)
#pragma warning (disable : 4100)

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"
#include "timer.h"

int main()
{
  HittableList world {};

  auto materialGround {std::make_shared<Lambertian>(Color {0.8, 0.8, 0.0})};
  auto materialCenter {std::make_shared<Lambertian>(Color {0.1, 0.2, 0.5})};
  auto materialLeft {std::make_shared<Dielectric>(1.50)};
  auto materialBubble {std::make_shared<Dielectric>(1.0  / 1.50)};
  auto materialRight {std::make_shared<Metal>(Color {0.8, 0.8, 0.8}, 0.3)};

  world.add(std::make_shared<Sphere>(Point3 {0.0, -100.5, -1.0}, 100.0, materialGround));
  world.add(std::make_shared<Sphere>(Point3 {0.0, 0.0, -1.2}, 0.5, materialCenter));
  world.add(std::make_shared<Sphere>(Point3 {-1.0, 0.0, -1.0}, 0.5, materialLeft));
  world.add(std::make_shared<Sphere>(Point3 {-1.0, 0.0, -1.0}, 0.4, materialBubble));
  world.add(std::make_shared<Sphere>(Point3 {1.0, 0.0, -1.0}, 0.5, materialRight));

  Camera cam {};

  cam.m_aspectRatio = (16.0 / 9.0);
  cam.m_imageWidth = 960; // 960x540
  cam.m_samplesPerPixel = 128;
  cam.m_maxDepth = 50;

  cam.m_vFov = 30;
  cam.m_lookFrom = Point3 {-2.0, 2.0, 1.0};
  cam.m_lookAt = Point3 {0.0, 0.0, -1.2};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 2.0;
  cam.m_focusDist = 3.4;

  cam.render(world);
  
  return 0;
}
