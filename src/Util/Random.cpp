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

glm::vec3 Random::Vec3() { return glm::vec3(Float(), Float(), Float()); }

glm::vec3 Random::Vec3(float min, float max) {
    return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
}

glm::vec3 Random::InSphere() {
    while (true) {
        glm::vec3 p = Vec3(-1, 1);
        float l = glm::length(p);
        if ((l * l) >= 1) continue;
        return p;
    }
}
}  // namespace raytracing