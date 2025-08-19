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

  auto groundMat {std::make_shared<Lambertian>(Color {0.5, 0.5, 0.5})};
  world.add(std::make_shared<Sphere>(Point3 {0.0, -1000.0, 0.0}, 1000.0, groundMat));

  for (int a {-11}; a < 11; ++a)
  {
    for (int b {-11}; b < 11; ++b)
    {
      auto chooseMat {randomDouble()};
      Point3 center {a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble()};

      if ((center - Point3 {4.0, 0.2, 0.0}).length() > 0.9)
      { 
        std::shared_ptr<Material> sphereMat;

        if (chooseMat < 0.8)
        {
          auto albedo {Color::random() * Color::random()};
          sphereMat = std::make_shared<Lambertian>(albedo);
          world.add(std::make_shared<Sphere>(center, 0, sphereMat));
        }
        else if (chooseMat < 0.95)
        {
          auto albedo {Color::random(0.5, 1.0)};
          auto fuzz {randomDouble(0.0, 0.5)};
          sphereMat = std::make_shared<Metal>(albedo, fuzz);
          world.add(std::make_shared<Sphere>(center, 0.2, sphereMat));
        }
        else
        {
          sphereMat = std::make_shared<Dielectric>(1.5);
          world.add(std::make_shared<Sphere>(center, 0.2, sphereMat));
        }
      }
    }
  }

  auto mat1 {std::make_shared<Dielectric>(1.5)};
  world.add(std::make_shared<Sphere>(Point3 {0.0, 1.0, 0.0}, 1.0, mat1));

  auto mat2 {std::make_shared<Lambertian>(Color {0.4, 0.2, 0.1})};
  world.add(std::make_shared<Sphere>(Point3 {-4.0, 1.0, 0.0}, 1.0, mat2));

  auto mat3 {std::make_shared<Metal>(Color {0.7, 0.6, 0.5}, 0.0)};
  world.add(std::make_shared<Sphere>(Point3 {4.0, 1.0, 0.0}, 1.0, mat3));

  Camera cam {};

  cam.m_aspectRatio = (16.0 / 9.0);
  cam.m_imageWidth = 1280; // 1280
  cam.m_samplesPerPixel = 512; // 512
  cam.m_maxDepth = 50;

  cam.m_vFov = 20;
  cam.m_lookFrom = Point3 {13.0, 2.0, 3.0};
  cam.m_lookAt = Point3 {0.0, 0.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0.6;
  cam.m_focusDist = 10.0;

  cam.render(world);
  
  return 0;
}
