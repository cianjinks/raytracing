#pragma once

#include "Object.h"

namespace raytracing {

class Intersection {
   public:
    static bool RaySphere(const Ray& ray, const Sphere& sphere);
    static bool RaySphere(const Ray& ray, const Sphere& sphere, float& r_t1,
                          float& r_t2);
};
}  // namespace raytracing