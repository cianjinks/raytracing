#pragma once

#include <glm/glm.hpp>
#include <random>

namespace raytracing {

class Random {
   private:
    static std::mt19937 s_Generator;

   public:
    /* Generate a uniform random double in the range [0, 1). */
    static double Double();
    /* Generate a uniform random double in the range [min, max). */
    static double Double(double min, double max);
    static float Float();
    static float Float(float min, float max);
    static glm::vec3 Vec3();
    static glm::vec3 Vec3(float min, float max);
    /* Generate a uniform random point in a sphere. */
    static glm::vec3 InSphere();
};

}  // namespace raytracing