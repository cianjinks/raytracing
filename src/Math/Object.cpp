#include "Object.h"

#include "Intersection.h"

namespace raytracing {

glm::vec3 Ray::At(float t) const { return origin + (direction * t); }

Scene::Scene(std::string name, glm::vec3 position) : Object(name, position) {}

Scene::~Scene() {
    for (Object* object : m_Objects) {
        delete object;
    }
    m_Objects.clear();
}

void Scene::Add(Object* object) { m_Objects.emplace_back(object); }

void Scene::Remove(Object* object) {
    m_Objects.erase(std::find(m_Objects.begin(), m_Objects.end(), object));
}

bool Scene::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    HitResult temp_hr;
    bool any_hit = false;
    float closest = t_max;
    for (Hittable* object : m_Objects) {
        if (object->Hit(ray, t_min, closest, temp_hr)) {
            any_hit = true;
            closest = temp_hr.t;
            hit = temp_hr;
        }
    }
    return any_hit;
}

bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    float t1, t2;
    bool result = Intersection::RaySphere(ray, *this, t1, t2);
    if (result) {
        /* Discard t values not between t_min and t_max. */
        float t = t1;
        if (t < t_min || t_max < t) {
            t = t2;
            if (t < t_min || t_max < t) {
                return false;
            }
        }

        hit.t = t;
        hit.position = ray.At(hit.t);
        hit.normal = (position - hit.position) / radius;

        /* Ensure normal goes outward from the surface. */
        bool face_test = glm::dot(ray.direction, hit.normal) < 0;
        hit.normal = face_test ? hit.normal : -hit.normal;
    }
    return result;
}

bool Box::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    // outNormal = -sign(rd) * step(t1.yzx, t1.xyz) * step(t1.zxy, t1.xyz);
    float t1, t2;
    glm::vec3 normal;
    bool result = Intersection::RayBox(ray, *this, t1, t2, normal);
    if (result) {
        /* Discard t values not between t_min and t_max. */
        float t = t1;
        if (t < t_min || t_max < t) {
            t = t2;
            if (t < t_min || t_max < t) {
                return false;
            }
        }

        hit.t = t;
        hit.position = ray.At(hit.t);
        hit.normal = normal;
    }
    return result;
}

}  // namespace raytracing