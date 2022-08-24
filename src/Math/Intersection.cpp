#include "Intersection.h"

namespace raytracing {

bool Intersection::RaySphere(const Ray& ray, const Sphere& sphere, float t_min,
                             float t_max, HitResult& hit) {
    glm::vec3 op = ray.origin - sphere.position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(ray.direction, op);
    float c = glm::dot(op, op) - (sphere.radius * sphere.radius);

    float discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) return false;

    float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
    float t2 = (-b - glm::sqrt(discriminant)) / (2 * a);
    t1 = glm::min(t1, t2);
    t2 = glm::max(t1, t2);

    float t = Constant::FInfinity;
    bool test = ClipT(t_min, t_max, t1, t2, t);
    if (!test) return false;

    hit.t = t;
    hit.position = ray.At(hit.t);
    hit.normal = (sphere.position - hit.position) / sphere.radius;
    hit.normal = EnsureNormal(ray, hit.normal);

    return true;
}

bool Intersection::RayBox(const Ray& ray, const Box& box, float t_min,
                          float t_max, HitResult& hit) {
    glm::vec3 ro = ray.origin - box.position;
    glm::vec3 m = 1.0f / ray.direction;
    glm::vec3 n = m * ro;
    glm::vec3 k = abs(m) * box.size;
    glm::vec3 t1v = -n - k;
    glm::vec3 t2v = -n + k;
    float t1 = glm::max(glm::max(t1v.x, t1v.y), t1v.z);
    float t2 = glm::min(glm::min(t2v.x, t2v.y), t2v.z);
    if (t1 > t2 || t2 < 0.0f) return false;
    glm::vec3 t1_shuffle1 = {t1v.y, t1v.z, t1v.x};
    glm::vec3 t1_shuffle2 = {t1v.z, t1v.x, t1v.y};

    float t = Constant::FInfinity;
    bool test = ClipT(t_min, t_max, t1, t2, t);
    if (!test) return false;

    hit.t = t;
    hit.position = ray.At(hit.t);
    hit.normal = -glm::sign(ray.direction) * glm::step(t1_shuffle1, t1v) *
                 glm::step(t1_shuffle2, t1v);
    hit.normal = EnsureNormal(ray, hit.normal);

    return true;
}

/* Discard if t values not between t_min and t_max. */
bool Intersection::ClipT(float t_min, float t_max, float t1, float t2,
                         float& r_t) {
    r_t = t1;
    if (r_t < t_min || t_max < r_t) {
        r_t = t2;
        if (r_t < t_min || t_max < r_t) {
            return false;
        }
    }
    return true;
}

/* Ensure normal goes outward from the surface. */
glm::vec3 Intersection::EnsureNormal(const Ray& ray, glm::vec3 normal) {
    bool face_test = glm::dot(ray.direction, normal) < 0;
    return face_test ? normal : -normal;
}

}  // namespace raytracing