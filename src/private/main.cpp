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

  cam.aspectRatio = (16.0 / 9.0);
  cam.imageWidth = 960;
  cam.samplesPerPixel = 100;

  cam.render(world);

  return 0;
}
