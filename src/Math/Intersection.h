#pragma once

#include "Object.h"

namespace raytracing {

class Intersection {
   public:
    static bool RaySphere(const Ray& ray, const Sphere& sphere);
    static bool RaySphere(const Ray& ray, const Sphere& sphere, float& r_t1,
                          float& r_t2);
    static bool RayBox(const Ray& ray, const Box& box, float& r_t1, float& r_t2,
                       glm::vec3& r_normal);
};
}  // namespace raytracing