#include "Math.h"

namespace raytracing {

glm::vec3 Math::V3Reflect(const glm::vec3& vec, const glm::vec3& normal) {
    float b = glm::dot(vec, normal);
    return vec - (2 * b * normal);
}

bool Math::V3NearZero(const glm::vec3& vec) {
    return (glm::abs(vec.x) < Constant::FMin) &&
           (glm::abs(vec.y) < Constant::FMin) &&
           (glm::abs(vec.z) < Constant::FMin);
}

}  // namespace raytracing