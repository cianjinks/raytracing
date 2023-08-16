#pragma once

#include <glm/glm.hpp>

namespace raytracing {

class Ray {
   public:
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 At(float t) const;
};

}