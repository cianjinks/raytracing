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
    hit.normal = (hit.position - sphere.position) / sphere.radius;
    hit.normal = EnsureNormal(ray, hit.normal);

    return true;
}

bool Intersection::Slabs(const Ray& ray, float t_min, float t_max, const glm::vec3& p0, const glm::vec3& p1) {
    glm::vec3 ird = 1.0f / ray.direction;
    glm::vec3 t0v = (p0 - ray.origin) * ird;
    glm::vec3 t1v = (p1 - ray.origin) * ird;
    glm::vec3 tminv = glm::min(t0v, t1v);
    glm::vec3 tmaxv = glm::max(t0v, t1v);
    float t1 = glm::max(glm::max(tminv.x, tminv.y), tminv.z);
    float t2 = glm::min(glm::min(tmaxv.x, tmaxv.y), tmaxv.z);

    float t = Constant::FInfinity;
    bool test = ClipT(t_min, t_max, t1, t2, t);
    if (!test) return false;

    return t1 <= t2;
}

/* RayBox is just Slabs but also computes normal and probably texture coords in the future. */
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

bool Intersection::RayRectangle(const Ray& ray, const Rectangle& rect, float t_min,
                                float t_max, HitResult& hit) {
    /* For a rectangle, the width and height must be perpendicular otherwise it will be a parallelogram. */
    if (glm::dot(rect.width, rect.height) != 0.0f) {
        return false;
    }

    glm::vec3 normal = glm::normalize(glm::cross(rect.width, rect.height));

    /* TODO: These intersection functions probably shouldn't take a Box/Plane/etc object. */
    HitResult plane_hit;
    Plane plane("", rect.position, nullptr, normal);
    if (!Intersection::RayPlane(ray, plane, t_min, t_max, plane_hit)) {
        return false;
    }

    /* See if intersection points lies within the rect bounds. */
    /* x = hit point */
    /* xw = project(x - p, w) */
    /* xh = project(x - p, h) */
    /* 0 <= length(xw) <= length(w) */
    /* 0 <= length(xh) <= length(h) */
    glm::vec3 xp = plane_hit.position - rect.position; /* We need hit pos relative to rect corner. */
    float w_l = glm::length(rect.width);
    float h_l = glm::length(rect.height);
    float xw_s = glm::dot(xp, rect.width) / w_l;
    float xh_s = glm::dot(xp, rect.height) / h_l;

    if (0 <= xw_s && xw_s <= w_l) {
        if (0 <= xh_s && xh_s <= h_l) {
            hit.t = plane_hit.t;
            hit.position = ray.At(hit.t);
            hit.normal = normal;
            hit.normal = EnsureNormal(ray, hit.normal);
            return true;
        }
    }
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
    bool face_test = glm::dot(ray.direction, normal) < 0; /* Negative dot product -> facing away from each other. */
    return face_test ? normal : -normal;
}

}  // namespace raytracing