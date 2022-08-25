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
    return Intersection::RaySphere(ray, *this, t_min, t_max, hit);
}

bool Box::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    return Intersection::RayBox(ray, *this, t_min, t_max, hit);
}

bool Plane::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    return Intersection::RayPlane(ray, *this, t_min, t_max, hit);
}

}  // namespace raytracing