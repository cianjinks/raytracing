#pragma once

#include <glm/glm.hpp>

#include "Constant.h"
#include "Ray.h"

namespace raytracing {

class BBox {
   private:
    glm::vec3 m_Min = glm::vec3(Constant::FMax);
    glm::vec3 m_Max = glm::vec3(Constant::FMin);
    bool m_Empty = true;

   public:
    BBox() {}
    BBox(const glm::vec3& p0, const glm::vec3& p1);
    BBox(const BBox& bbox0, const BBox& bbox1);

    bool Pad();

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max) const;

    bool Empty() const { return m_Empty; };

    const glm::vec3& min() const { return m_Min; }
    const glm::vec3& max() const { return m_Max; }
};

}  // namespace raytracing