#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "constantMedium.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

void bouncingSpheres()
{
  HittableList world {};

  auto checker {std::make_shared<CheckerTexture>(0.32, Color {0.2, 0.3, 0.1}, Color {0.9, 0.9, 0.9})};
  world.add(std::make_shared<Sphere>(Point3 {0.0, -1000.0, 0.0}, 1000.0, std::make_shared<Lambertian>(checker)));

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
          auto center2 {center + Vec3 {0.0, randomDouble(0.0, 0.5), 0.0}};
          world.add(std::make_shared<Sphere>(center, center2, 0.2, sphereMat));
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

  world = HittableList {std::make_shared<BVHNode>(world)};

  Camera cam {};

  cam.m_aspectRatio = (16.0 / 9.0);
  cam.m_imageWidth = 1280; // 1280
  cam.m_samplesPerPixel = 512; // 512
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.70, 0.80, 1.00};

  cam.m_vFov = 20;
  cam.m_lookFrom = Point3 {13.0, 2.0, 3.0};
  cam.m_lookAt = Point3 {0.0, 0.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0.6;
  cam.m_focusDist = 10.0;

  cam.render(world);
}

void CheckeredSpheres()
{
  HittableList world;

  auto checker {std::make_shared<CheckerTexture>(0.32, Color {0.2, 0.3, 0.1}, Color {0.9, 0.9, 0.9})};
  world.add(std::make_shared<Sphere>(Point3 {0.0, -10.0, 0.0}, 10.0, std::make_shared<Lambertian>(checker)));
  world.add(std::make_shared<Sphere>(Point3 {0.0, 10.0, 0.0}, 10.0, std::make_shared<Lambertian>(checker)));

  Camera cam;

  cam.m_aspectRatio = 16.0 / 9.0;
  cam.m_imageWidth = 1280;
  cam.m_samplesPerPixel = 512; // 512
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.70, 0.80, 1.00};

  cam.m_vFov = 20;
  cam.m_lookFrom = Point3 {13.0, 2.0, 3.0};
  cam.m_lookAt = Point3 {0.0, 0.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0.6;
  cam.m_focusDist = 10.0;

  cam.render(world);
}

void earth()
{
  auto earthTexture {std::make_shared<ImageTexture>("earth.jpg")};
  auto earthSurface {std::make_shared<Lambertian>(earthTexture)};
  auto globe {std::make_shared<Sphere>(Point3 {0.0, 0.0, 0.0}, 2.0, earthSurface)};

  Camera cam;

  cam.m_aspectRatio = 16.0 / 9.0;
  cam.m_imageWidth = 1280; // 1280
  cam.m_samplesPerPixel = 512; // 512
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.70, 0.80, 1.00};

  cam.m_vFov = 20;
  cam.m_lookFrom = Point3 {0.0, 0.0, 12.0};
  cam.m_lookAt = Point3 {0.0, 0.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0.0;

  cam.render(HittableList {globe});
}

void perlinSpheres()
{
  HittableList world;

  auto pertext {std::make_shared<NoiseTexture>(4)};
  world.add(std::make_shared<Sphere>(Point3 {0.0, -1000.0, 0.0}, 1000.0, std::make_shared<Lambertian>(pertext)));
  world.add(std::make_shared<Sphere>(Point3 {0.0, 2.0, 0.0}, 2.0, std::make_shared<Lambertian>(pertext)));

  Camera cam;

  cam.m_aspectRatio = 16.0 / 9.0;
  cam.m_imageWidth = 1280; // 1280x720
  cam.m_samplesPerPixel = 512; // 512
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.70, 0.80, 1.00};

  cam.m_vFov = 20;
  cam.m_lookFrom = Point3 {13.0, 2.0, 3.0};
  cam.m_lookAt = Point3 {0.0, 0.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0.0;

  cam.render(world);
}

void quads()
{
  HittableList world;

  auto leftRed {std::make_shared<Lambertian>(Color {1.0, 0.2, 0.2})};
  auto backGreen {std::make_shared<Lambertian>(Color {0.2, 1.0, 0.2})};
  auto rightBlue {std::make_shared<Lambertian>(Color {0.2, 0.2, 1.0})};
  auto upperOrange {std::make_shared<Lambertian>(Color {1.0, 0.5, 0.0})};
  auto lowerTeal {std::make_shared<Lambertian>(Color {0.2, 0.8, 0.8})};

  world.add(std::make_shared<Quad>(Point3 {-3.0, -2.0, 5.0}, Vec3 {0.0, 0.0, -4.0}, Vec3 {0.0, 4.0, 0.0}, leftRed));
  world.add(std::make_shared<Quad>(Point3 {-2.0, -2.0, 0.0}, Vec3 {4.0, 0.0, 0.0}, Vec3 {0.0, 4.0, 0.0}, backGreen));
  world.add(std::make_shared<Quad>(Point3 {3.0, -2.0, 1.0}, Vec3 {0.0, 0.0, 4.0}, Vec3 {0.0, 4.0, 0.0}, rightBlue));
  world.add(std::make_shared<Quad>(Point3 {-2.0, 3.0, 1.0}, Vec3 {4.0, 0.0, 0.0}, Vec3 {0.0, 0.0, 4.0}, upperOrange));
  world.add(std::make_shared<Quad>(Point3 {-2.0, -3.0, 5.0}, Vec3 {4.0, 0.0, 0.0}, Vec3 {0.0, 0.0, -4.0}, lowerTeal));

  Camera cam;

  cam.m_aspectRatio = 1.0;
  cam.m_imageWidth = 320;
  cam.m_samplesPerPixel = 32;
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.70, 0.80, 1.00};

  cam.m_vFov = 80;
  cam.m_lookFrom = Point3 {0.0, 0.0, 9.0};
  cam.m_lookAt = Point3 {0.0, 0.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0.0;

  cam.render(world);
}

void simpleLight()
{
  HittableList world;

  auto pertext {std::make_shared<NoiseTexture>(4)};
  world.add(std::make_shared<Sphere>(Point3 {0.0, -1000.0, 0.0}, 1000.0, std::make_shared<Lambertian>(pertext)));
  world.add(std::make_shared<Sphere>(Point3 {0.0, 2.0, 0.0}, 2.0, std::make_shared<Lambertian>(pertext)));

  auto diffLight {std::make_shared<DiffuseLight>(Color {4.0, 4.0, 4.0})};
  world.add(std::make_shared<Sphere>(Point3 {0.0, 7.0, 0.0}, 2.0, diffLight));
  world.add(std::make_shared<Quad>(Point3 {3.0, 1.0, -2.0}, Vec3 {2.0, 0.0, 0.0}, Vec3 {0.0, 2.0, 0.0}, diffLight));

  Camera cam;

  cam.m_aspectRatio = 16.0 / 9.0;
  cam.m_imageWidth = 640;
  cam.m_samplesPerPixel = 32;
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.0, 0.0, 0.0};

  cam.m_vFov = 20;
  cam.m_lookFrom = Point3 {26.0, 3.0, 6.0};
  cam.m_lookAt = Point3 {0.0, 2.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0;

  cam.render(world);
}

void cornellBox()
{
  HittableList world;

  auto red {std::make_shared<Lambertian>(Color {0.65, 0.05, 0.05})};
  auto white {std::make_shared<Lambertian>(Color {0.73, 0.73, 0.73})};
  auto green {std::make_shared<Lambertian>(Color {0.12, 0.45, 0.15})};
  auto light {std::make_shared<DiffuseLight>(Color {15.0, 15.0, 15.0})};

  world.add(std::make_shared<Quad>(Point3 {555.0, 0.0, 0.0}, Vec3 {0.0, 555.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, green));
  world.add(std::make_shared<Quad>(Point3 {0.0, 0.0, 0.0}, Vec3 {0.0, 555.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, red));
  world.add(std::make_shared<Quad>(Point3 {343.0, 554.0, 332.0}, Vec3 {-130.0, 0.0, 0.0}, Vec3 {0.0, 0.0, -105.0}, light));
  world.add(std::make_shared<Quad>(Point3 {0.0, 0.0, 0.0}, Vec3 {555.0, 0.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, white));
  world.add(std::make_shared<Quad>(Point3 {555.0, 555.0, 555.0}, Vec3 {-555.0, 0.0, 0.0}, Vec3 {0.0, 0.0, -555.0}, white));
  world.add(std::make_shared<Quad>(Point3 {0.0, 0.0, 555.0}, Vec3 {555.0, 0.0, 0.0}, Vec3 {0.0, 555.0, 0.0}, white));

  std::shared_ptr<Hittable> box1 {box(Point3 {0.0, 0.0, 0.0}, Point3 {165.0, 330.0, 165.0}, white)};
  box1 = std::make_shared<RotateY>(box1, 15);
  box1 = std::make_shared<Translate>(box1, Vec3 {265.0, 0.0, 295.0});
  world.add(box1);

  std::shared_ptr<Hittable> box2 {box(Point3 {0.0, 0.0, 0.0}, Point3 {165.0, 165.0, 165.0}, white)};
  box2 = std::make_shared<RotateY>(box2, -18);
  box2 = std::make_shared<Translate>(box2, Vec3 {130.0, 0.0, 65.0});
  world.add(box2);

  Camera cam;

  cam.m_aspectRatio = 1.0;
  cam.m_imageWidth = 720;
  cam.m_samplesPerPixel = 196;
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.0, 0.0, 0.0};

  cam.m_vFov = 40;
  cam.m_lookFrom = Point3 {278.0, 278.0, -800.0};
  cam.m_lookAt = Point3 {278.0, 278.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0;

  cam.render(world);
}

void cornellSmoke()
{
  HittableList world;

  auto red {std::make_shared<Lambertian>(Color {0.65, 0.05, 0.05})};
  auto white {std::make_shared<Lambertian>(Color {0.73, 0.73, 0.73})};
  auto green {std::make_shared<Lambertian>(Color {0.12, 0.45, 0.15})};
  auto light {std::make_shared<DiffuseLight>(Color {7.0, 7.0, 7.0})};

  world.add(std::make_shared<Quad>(Point3 {555.0, 0.0, 0.0}, Vec3 {0.0, 555.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, green));
  world.add(std::make_shared<Quad>(Point3 {0.0, 0.0, 0.0}, Vec3 {0.0, 555.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, red));
  world.add(std::make_shared<Quad>(Point3 {113.0, 554.0, 127.0}, Vec3 {330.0, 0.0, 0.0}, Vec3 {0.0, 0.0, 305.0}, light));
  world.add(std::make_shared<Quad>(Point3 {0.0, 555.0, 0.0}, Vec3 {555.0, 0.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, white));
  world.add(std::make_shared<Quad>(Point3 {0.0, 0.0, 0.0}, Vec3 {555.0, 0.0, 0.0}, Vec3 {0.0, 0.0, 555.0}, white));
  world.add(std::make_shared<Quad>(Point3 {0.0, 0.0, 555.0}, Vec3 {555.0, 0.0, 0.0}, Vec3 {0.0, 555.0, 0.0}, white));

  std::shared_ptr<Hittable> box1 {box(Point3 {0.0, 0.0, 0.0}, Point3 {165.0, 330.0, 165.0}, white)};
  box1 = std::make_shared<RotateY>(box1, 15);
  box1 = std::make_shared<Translate>(box1, Vec3 {265.0, 0.0, 295.0});

  std::shared_ptr<Hittable> box2 {box(Point3 {0.0, 0.0, 0.0}, Point3 {165.0, 165.0, 165.0}, white)};
  box2 = std::make_shared<RotateY>(box2, -18);
  box2 = std::make_shared<Translate>(box2, Vec3 {130.0, 0.0, 65.0});

  world.add(std::make_shared<ConstantMedium>(box1, 0.01, Color {0.0, 0.0, 0.0}));
  world.add(std::make_shared<ConstantMedium>(box2, 0.01, Color {1.0, 1.0, 1.0}));

  Camera cam;

  cam.m_aspectRatio = 1.0;
  cam.m_imageWidth = 900;
  cam.m_samplesPerPixel = 512;
  cam.m_maxDepth = 50;
  cam.m_background = Color {0.0, 0.0, 0.0};

  cam.m_vFov = 40;
  cam.m_lookFrom = Point3 {278.0, 278.0, -800.0};
  cam.m_lookAt = Point3 {278.0, 278.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0;

  cam.render(world);
}

int main()
{
  switch (8)
  {
    case 1:
      bouncingSpheres();
      break;
    case 2:
      CheckeredSpheres();
      break;
    case 3:
      earth();
      break;
    case 4:
      perlinSpheres();
      break;
    case 5:
      quads();
      break;
    case 6:
      simpleLight();
      break;
    case 7:
      cornellBox();
      break;
    case 8:
      cornellSmoke();
      break;
  }

  return 0;
}
