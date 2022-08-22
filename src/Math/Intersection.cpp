#include "Intersection.h"

namespace raytracing {

bool Intersection::RaySphere(const Ray& ray, const Sphere& sphere) {
    // b^2 - 4ac < 0 -> 0 solutions
    glm::vec3 d = ray.direction * ray.direction;
    float a = d.x + d.y + d.z;
    glm::vec3 od = ray.origin * ray.direction;
    float b = 2 * (od.x + od.y + od.z);
    glm::vec3 o = ray.origin * ray.origin;
    float c = o.x + o.y + o.z - (sphere.radius * sphere.radius);

    float discriminant = (b * b) - (4 * a * c);
    return discriminant >= 0;
}

bool Intersection::RaySphere(const Ray& ray, const Sphere& sphere,
                             glm::vec3& r_hit) {}

}  // namespace raytracing