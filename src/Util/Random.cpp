#include "Random.h"

#include <random>

namespace raytracing {

std::mt19937 Random::s_Generator;

double Random::Double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(s_Generator);
}

double Random::Double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    return distribution(s_Generator);
}

float Random::Float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(s_Generator);
}

float Random::Float(float min, float max) {
    static std::uniform_real_distribution<float> distribution(min, max);
    return distribution(s_Generator);
}
}  // namespace raytracing