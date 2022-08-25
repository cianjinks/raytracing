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
    glm::vec3 ird = 1.0f / ray.direction;
    glm::vec3 p0 = box.position - box.size;
    glm::vec3 p1 = box.position + box.size;
    glm::vec3 t0v = (p0 - ray.origin) * ird;
    glm::vec3 t1v = (p1 - ray.origin) * ird;
    glm::vec3 tminv = glm::min(t0v, t1v);
    glm::vec3 tmaxv = glm::max(t0v, t1v);
    float t1 = glm::max(glm::max(tminv.x, tminv.y), tminv.z);
    float t2 = glm::min(glm::min(tmaxv.x, tmaxv.y), tmaxv.z);
    if (t1 > t2) { /* || t2 < 0.0f */
        return false;
    }

    float t = Constant::FInfinity;
    bool test = ClipT(t_min, t_max, t1, t2, t);
    if (!test) return false;

    hit.t = t;
    hit.position = ray.At(hit.t);
    glm::vec3 t1_shuffle1 = {tminv.y, tminv.z, tminv.x};
    glm::vec3 t1_shuffle2 = {tminv.z, tminv.x, tminv.y};
    hit.normal = -glm::sign(ray.direction) * glm::step(t1_shuffle1, tminv) *
                 glm::step(t1_shuffle2, tminv);
    hit.normal = EnsureNormal(ray, hit.normal);

    return true;
}

bool Intersection::RayPlane(const Ray& ray, const Plane& plane, float t_min,
                            float t_max, HitResult& hit) {
    float nd = glm::dot(plane.normal, ray.direction);
    float no = glm::dot(plane.normal, ray.origin);
    float np = glm::dot(plane.normal, plane.position);
    if (nd == 0.0f) return false;

    float t = (np - no) / nd;
    if (t < t_min || t_max < t) return false;

    hit.t = t;
    hit.position = ray.At(hit.t);
    hit.normal = plane.normal;
    hit.normal = EnsureNormal(ray, hit.normal);
    return true;
}

bool Intersection::RayCylinder(const Ray& ray, const Cylinder& cylinder,
                               float t_min, float t_max, HitResult& hit) {
    /* TODO. */
    return false;
}

bool Intersection::RayTorus(const Ray& ray, const Torus& torus, float t_min,
                            float t_max, HitResult& hit) {
    /* TODO. */
    return false;
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