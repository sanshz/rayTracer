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

void finalScene(int imageWidth, int samplesPerPixel, int maxDepth)
{
  HittableList boxes1;
  auto ground {std::make_shared<Lambertian>(Color {0.48, 0.83, 0.53})};

  int boxesPerSide {20};
  for (int i {0}; i < boxesPerSide; ++i)
  {
    for (int j {0}; j < boxesPerSide; ++j)
    {
      auto w {100.0};
      auto x0 {-1000.0 + i * w};
      auto y0 {0.0};
      auto z0 {-1000.0 + j * w};
      auto x1 {x0 + w};
      auto y1 {randomDouble(1.0, 101.0)};
      auto z1 {z0 + w};

      boxes1.add(box(Point3 {x0, y0, z0}, Point3 {x1, y1, z1}, ground));
    }
  }

  HittableList world;
  world.add(std::make_shared<BVHNode>(boxes1));

  auto light {std::make_shared<DiffuseLight>(Color {7.0, 7.0, 7.0})};
  world.add(std::make_shared<Quad>(Point3 {123.0, 554.0, 147.0}, Vec3 {300.0, 0.0, 0.0}, Vec3 {0.0, 0.0, 265.0}, light));

  auto center1 {Point3 {400.0, 400.0, 200.0}};
  auto center2 {center1 + Vec3 {30.0, 0.0, 0.0}};
  auto sphereMat {std::make_shared<Lambertian>(Color {0.7, 0.3, 0.1})};
  world.add(std::make_shared<Sphere>(center1, center2, 50, sphereMat));

  world.add(make_shared<Sphere>(Point3 {260.0, 150.0, 45.0}, 50.0, std::make_shared<Dielectric>(1.5)));
  world.add(std::make_shared<Sphere>(Point3 {0.0, 150.0, 145.0}, 50.0, std::make_shared<Metal>(Color {0.8, 0.8, 0.9}, 1.0)));

  auto boundary {std::make_shared<Sphere>(Point3 {360.0, 150.0, 145.0}, 70.0, std::make_shared<Dielectric>(1.5))};
  world.add(boundary);
  world.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color {0.2, 0.4, 0.9}));
  boundary = std::make_shared<Sphere>(Point3 {0.0, 0.0, 0.0}, 5000.0, std::make_shared<Dielectric>(1.5));
  world.add(std::make_shared<ConstantMedium>(boundary, 0.0001, Color {1.0, 1.0, 1.0}));

  auto eMat {std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earth.jpg"))};
  world.add(std::make_shared<Sphere>(Point3 {400.0, 200.0, 400.0}, 100.0, eMat));
  auto pertext {std::make_shared<NoiseTexture>(0.2)};
  world.add(std::make_shared<Sphere>(Point3 {220.0, 280.0, 300.0}, 80.0, std::make_shared<Lambertian>(pertext)));

  HittableList boxes2;
  auto white {std::make_shared<Lambertian>(Color {0.73, 0.73, 0.73})};
  int ns {1000};
  for (int j {0}; j < ns; ++j)
  { boxes2.add(std::make_shared<Sphere>(Point3::random(0.0, 165), 10.0, white)); }

  world.add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes2), 15), Vec3 {-100.0, 270.0, 395.0}));

  Camera cam;
  cam.m_aspectRatio = 1.0;
  cam.m_imageWidth = imageWidth;
  cam.m_samplesPerPixel = samplesPerPixel;
  cam.m_maxDepth = maxDepth;
  cam.m_background = Color {0.0, 0.0, 0.0};

  cam.m_vFov = 40;
  cam.m_lookFrom = Point3 {478.0, 278.0, -600.0};
  cam.m_lookAt = Point3 {278.0, 278.0, 0.0};
  cam.m_vUp = Vec3 {0.0, 1.0, 0.0};

  cam.m_defocusAngle = 0;

  cam.render(world);
}

int main()
{
  finalScene(900, 1024, 40);

  return 0;
}
