#include "Math.h"

namespace raytracing {

bool Math::V3NearZero(const glm::vec3 vec) {
    return (glm::abs(vec.x) < Constant::FMin) && (glm::abs(vec.y) < Constant::FMin) && (glm::abs(vec.z) < Constant::FMin);
}

}  // namespace raytracing