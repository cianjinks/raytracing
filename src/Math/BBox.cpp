#include "BBox.h"

#include "Intersection.h"

namespace raytracing {

BBox::BBox(const glm::vec3& p0, const glm::vec3& p1) {
    min.x = glm::min(p0.x, p1.x);
    min.y = glm::min(p0.y, p1.y);
    min.z = glm::min(p0.z, p1.z);

    max.x = glm::max(p0.x, p1.x);
    max.y = glm::max(p0.y, p1.y);
    max.z = glm::max(p0.z, p1.z);
}

BBox::BBox(const BBox& bbox0, const BBox& bbox1) {
    min.x = glm::min(bbox0.min.x, bbox1.min.x);
    min.y = glm::min(bbox0.min.y, bbox1.min.y);
    min.z = glm::min(bbox0.min.z, bbox1.min.z);

    max.x = glm::max(bbox0.max.x, bbox1.max.x);
    max.y = glm::max(bbox0.max.y, bbox1.max.y);
    max.z = glm::max(bbox0.max.z, bbox1.max.z);
}

bool BBox::Hit(const Ray& ray, float t_min, float t_max) const {
    return Intersection::Slabs(ray, t_min, t_max, min, max);
}

}  // namespace raytracing