#pragma once

#include <random>

namespace raytracing {

class Random {
   private:
    static std::mt19937 s_Generator;

   public:
    /* Generate a random double in the range [0, 1). */
    static double Double();
    /* Generate a random double in the range [min, max). */
    static double Double(double min, double max);
    /* Generate a random float in the range [0, 1). */
    static float Float();
    /* Generate a random float in the range [min, max). */
    static float Float(float min, float max);
};

}  // namespace raytracing