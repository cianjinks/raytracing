#include "Intersection.h"

namespace raytracing {

bool Intersection::RaySphere(const Ray& ray, const Sphere& sphere) {
    glm::vec3 op = ray.origin - sphere.position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(ray.direction, op);
    float c = glm::dot(op, op) - (sphere.radius * sphere.radius);

    // b^2 - 4ac < 0 -> 0 solutions
    float discriminant = (b * b) - (4 * a * c);
    return discriminant >= 0;
}

bool Intersection::RaySphere(const Ray& ray, const Sphere& sphere, float& r_t1,
                             float& r_t2) {
    glm::vec3 op = ray.origin - sphere.position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(ray.direction, op);
    float c = glm::dot(op, op) - (sphere.radius * sphere.radius);

    float discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) return false;

    float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
    float t2 = (-b - glm::sqrt(discriminant)) / (2 * a);
    r_t1 = glm::min(t1, t2);
    r_t2 = glm::max(t1, t2);
    return true;
}

bool Intersection::RayBox(const Ray& ray, const Box& box, float& r_t1,
                          float& r_t2, glm::vec3& normal) {
    glm::vec3 ro = ray.origin - box.position;
    glm::vec3 m = 1.0f / ray.direction;
    glm::vec3 n = m * ro;
    glm::vec3 k = abs(m) * box.size;
    glm::vec3 t1 = -n - k;
    glm::vec3 t2 = -n + k;
    r_t1 = glm::max(glm::max(t1.x, t1.y), t1.z);
    r_t2 = glm::min(glm::min(t2.x, t2.y), t2.z);
    if (r_t1 > r_t2 || r_t2 < 0.0f) return false;
    glm::vec3 t1_shuffle1 = {t1.y, t1.z, t1.x};
    glm::vec3 t1_shuffle2 = {t1.z, t1.x, t1.y};
    normal = -glm::sign(ray.direction) * glm::step(t1_shuffle1, t1) *
             glm::step(t1_shuffle2, t1);
    return true;
}

}  // namespace raytracing