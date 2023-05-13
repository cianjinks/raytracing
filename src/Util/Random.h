#pragma once

#include <glm/glm.hpp>
#include <random>

namespace raytracing {

class Random {
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
    /* Generate a uniform random point in a hemisphere around a normal. */
    static glm::vec3 InHemisphere(const glm::vec3& normal);
    /* Generate a uniform random point in a unit disk. */
    static glm::vec3 InUnitDisk();

   private:
    static std::mt19937& Generator();
    static std::uniform_real_distribution<double>& DoubleDist();
    static std::uniform_real_distribution<float>& FloatDist();
};

}  // namespace raytracing