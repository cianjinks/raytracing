#pragma once

#include <glm/glm.hpp>

#include "Constant.h"
#include "Ray.h"

namespace raytracing {

class BBox {
   public:
    /* TODO: Should empty bounding box be infinity? */
    glm::vec3 min = glm::vec3(0.0f);
    glm::vec3 max = glm::vec3(0.0f);

   public:
    BBox() {}
    BBox(const glm::vec3& p0, const glm::vec3& p1);
    BBox(const BBox& bbox0, const BBox& bbox1);

    bool Hit(const Ray& ray, float t_min, float t_max) const;
};

}  // namespace raytracing