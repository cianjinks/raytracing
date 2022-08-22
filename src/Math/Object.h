#pragma once

#include <glm/glm.hpp>

namespace raytracing {

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Sphere {
    glm::vec3 position;
    float radius;
};

}  // namespace raytracing