#include "Math.h"

namespace raytracing {

glm::vec3 Math::V3Reflect(const glm::vec3& vec, const glm::vec3& normal) {
    float b = glm::dot(vec, normal);
    return vec - (2 * b * normal);
}

/* TODO: Prove and verify. */
glm::vec3 Math::V3Refract(const glm::vec3& vec, const glm::vec3& normal, float ratio)
{
    float cos_theta = glm::min(glm::dot(-vec, normal), 1.0f);
    glm::vec3 r_out_perp = ratio * (vec + cos_theta*normal);
    glm::vec3 r_out_parallel = -glm::sqrt(glm::abs(1.0f - (glm::length(r_out_perp) * glm::length(r_out_perp)))) * normal;
    return r_out_perp + r_out_parallel;
}

bool Math::V3NearZero(const glm::vec3& vec) {
    return (glm::abs(vec.x) < Constant::FMin) &&
           (glm::abs(vec.y) < Constant::FMin) &&
           (glm::abs(vec.z) < Constant::FMin);
}

}  // namespace raytracing