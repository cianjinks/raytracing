#pragma once

#include <glm/glm.hpp>

#include "Constant.h"

namespace raytracing {

/* Class for extra math utility functions. */
class Math {
   public:
    static bool V3NearZero(const glm::vec3 vec);
};
}  // namespace raytracing