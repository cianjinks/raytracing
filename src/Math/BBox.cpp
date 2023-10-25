#include "BBox.h"

#include "Intersection.h"

namespace raytracing {

BBox::BBox(const glm::vec3& p0, const glm::vec3& p1) {
    m_Min.x = glm::min(p0.x, p1.x);
    m_Min.y = glm::min(p0.y, p1.y);
    m_Min.z = glm::min(p0.z, p1.z);

    m_Max.x = glm::max(p0.x, p1.x);
    m_Max.y = glm::max(p0.y, p1.y);
    m_Max.z = glm::max(p0.z, p1.z);

    m_Empty = false;
}

BBox::BBox(const BBox& bbox0, const BBox& bbox1) {
    m_Min.x = glm::min(bbox0.m_Min.x, bbox1.m_Min.x);
    m_Min.y = glm::min(bbox0.m_Min.y, bbox1.m_Min.y);
    m_Min.z = glm::min(bbox0.m_Min.z, bbox1.m_Min.z);

    m_Max.x = glm::max(bbox0.m_Max.x, bbox1.m_Max.x);
    m_Max.y = glm::max(bbox0.m_Max.y, bbox1.m_Max.y);
    m_Max.z = glm::max(bbox0.m_Max.z, bbox1.m_Max.z);

    m_Empty = false;
}

bool BBox::Pad() {
    bool padded = false;
    float delta = 0.0001f;
    if ((m_Max.x - m_Min.x) < delta) {
        m_Min.x -= delta;
        m_Max.x += delta;
        padded = true;
    }
    if ((m_Max.y - m_Min.y) < delta) {
        m_Min.y -= delta;
        m_Max.y += delta;
        padded = true;
    }
    if ((m_Max.z - m_Min.z) < delta) {
        m_Min.z -= delta;
        m_Max.z += delta;
        padded = true;
    }
    return padded;
}

bool BBox::Hit(const Ray& ray, float t_min, float t_max) const {
    return Intersection::Slabs(ray, t_min, t_max, m_Min, m_Max);
}

}  // namespace raytracing