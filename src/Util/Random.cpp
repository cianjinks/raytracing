#include "Random.h"

#include <random>

namespace raytracing {

/* https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers
 */
std::mt19937& Random::Generator() {
    static thread_local std::mt19937 generator;
    return generator;
}

double Random::Double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(Generator());
}

double Random::Double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    return distribution(Generator());
}

float Random::Float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(Generator());
}

float Random::Float(float min, float max) {
    static std::uniform_real_distribution<float> distribution(min, max);
    return distribution(Generator());
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

glm::vec3 Random::InHemisphere(const glm::vec3& normal) {
    glm::vec3 in_sphere = InSphere();
    if (glm::dot(in_sphere, normal) > 0.0f)
        return in_sphere;
    else
        return -in_sphere;
}

glm::vec3 Random::InUnitDisk() {
    while (true) {
        glm::vec3 p = glm::vec3(Float(-1, 1), Float(-1, 1), 0);
        float l = glm::length(p);
        if ((l * l) >= 1) continue;
        return p;
    }
}
}  // namespace raytracing