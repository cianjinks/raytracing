#include "Random.h"

#include <random>

#include "Math/Constant.h"

namespace raytracing {

// FAST RANDOM
float FastRandom::Float(uint32_t& seed) {
    return float(PCGHash(seed)) / float(std::numeric_limits<uint32_t>::max());
}

float FastRandom::Float(uint32_t& seed, float min, float max) {
    float r = Float(seed);
    return (r * (max - min)) + min;
}

glm::vec3 FastRandom::Vec3(uint32_t& seed) {
    return glm::vec3(Float(seed), Float(seed), Float(seed));
}

glm::vec3 FastRandom::Vec3(uint32_t& seed, float min, float max) {
    return glm::vec3(Float(seed, min, max), Float(seed, min, max), Float(seed, min, max));
}

uint8_t FastRandom::U8(uint32_t& seed) {
    // TODO: Is this uniformly distributed? Also how does C++ handle cast from 32 bit to 8 bit integer?
    return (uint8_t)PCGHash(seed);
}

glm::vec3 FastRandom::InSphere(uint32_t& seed) {
    while (true) {
        glm::vec3 p = Vec3(seed, -1.0f, 1.0f);
        float l = p.x * p.x + p.y * p.y + p.z * p.z;
        if (l >= 1) continue;
        return p;
    }
}

glm::vec3 FastRandom::InHemisphere(uint32_t& seed, const glm::vec3& normal) {
    glm::vec3 in_sphere = InSphere(seed);
    if (glm::dot(in_sphere, normal) > 0.0f)
        return in_sphere;
    else
        return -in_sphere;
}

glm::vec3 FastRandom::InUnitDisk(uint32_t& seed) {
    while (true) {
        glm::vec3 p = glm::vec3(Float(seed, -1.0f, 1.0f), Float(seed, -1.0f, 1.0f), 0.0f);
        float l = glm::length(p);
        if ((l * l) >= 1) continue;
        return p;
    }
}

uint32_t FastRandom::GetTimeSeed() {
    // TODO: Probably not good practice but works for now.
    return (uint32_t)std::clock();
}

uint32_t FastRandom::PCGHash(uint32_t& seed) {
    uint32_t state = seed;
    seed = seed * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

// SLOW RANDOM
/* https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers
 */
std::mt19937& Random::Generator() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 generator(rd());
    return generator;
}

std::uniform_real_distribution<double>& Random::DoubleDist() {
    static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution;
}

std::uniform_real_distribution<float>& Random::FloatDist() {
    static thread_local std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution;
}

std::uniform_int_distribution<uint32_t> Random::IntDist() {
    static thread_local std::uniform_int_distribution<uint32_t> distribution(0, 1);
    return distribution;
}

std::uniform_real_distribution<double> Random::DoubleDist(double min, double max) {
    return std::uniform_real_distribution<double>(min, max);
}

std::uniform_real_distribution<float> Random::FloatDist(float min, float max) {
    return std::uniform_real_distribution<float>(min, max);
}

std::uniform_int_distribution<uint32_t> Random::IntDist(int min, int max) {
    return std::uniform_int_distribution<uint32_t>(min, max);
}

double Random::Double() {
    return DoubleDist()(Generator());
}

/* TODO: Do I need the fmod? */
double Random::Double(double min, double max) {
    double val = DoubleDist(min, max)(Generator());
    val = std::fmod(val, max - min) + min;
    return val;
}

float Random::Float() {
    return FloatDist()(Generator());
}

/* TODO: Do I need the fmod? */
float Random::Float(float min, float max) {
    float val = FloatDist(min, max)(Generator());
    val = std::fmod(val, max - min) + min;
    return val;
}

uint32_t Random::Int() {
    return IntDist()(Generator());
}

uint32_t Random::Int(uint32_t min, uint32_t max) {
    return IntDist(min, max)(Generator());
}

glm::vec3 Random::Vec3() { return glm::vec3(Float(), Float(), Float()); }

glm::vec3 Random::Vec3(float min, float max) {
    return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
}

glm::vec3 Random::InSphere() {
    while (true) {
        glm::vec3 p = Vec3(-1, 1);
        float l = p.x * p.x + p.y * p.y + p.z * p.z;
        if (l >= 1) continue;
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