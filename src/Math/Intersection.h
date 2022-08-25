#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Constant.h"
#include "Object.h"

namespace raytracing {

class Intersection {
   public:
    static bool RaySphere(const Ray& ray, const Sphere& sphere, float t_min,
                          float t_max, HitResult& hit);
    static bool RayBox(const Ray& ray, const Box& box, float t_min, float t_max,
                       HitResult& hit);

    static bool ClipT(float t_min, float t_max, float t1, float t2, float& r_t);
    static glm::vec3 EnsureNormal(const Ray& ray, glm::vec3 normal);
};
}  // namespace raytracing