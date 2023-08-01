#include "Object.h"

#include "Intersection.h"
#include "Material.h"
#include "Window/UI.h"

namespace raytracing {

glm::vec3 Ray::At(float t) const { return origin + (direction * t); }

bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    if (Intersection::RaySphere(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Box::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    if (Intersection::RayBox(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Plane::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    if (Intersection::RayPlane(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Cylinder::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    if (Intersection::RayCylinder(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Torus::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    if (Intersection::RayTorus(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

}  // namespace raytracing