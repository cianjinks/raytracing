#pragma once

#include <glm/glm.hpp>

#include "Constant.h"

namespace raytracing {

/* Class for extra math utility functions. */
class Math {
   public:
    static glm::vec3 V3Reflect(const glm::vec3& vec, const glm::vec3& normal);
    static glm::vec3 V3Refract(const glm::vec3& vec, const glm::vec3& normal, float ratio);
    
    static bool V3NearZero(const glm::vec3& vec);
};
}  // namespace raytracing