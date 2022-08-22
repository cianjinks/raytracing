#pragma once

#include "Object.h"

namespace raytracing {

class Intersection {
   public:
    static bool RaySphere(const Ray& ray, const Sphere& sphere);
    static bool RaySphere(const Ray& ray, const Sphere& sphere,
                          glm::vec3& r_hit);
};
}  // namespace raytracing