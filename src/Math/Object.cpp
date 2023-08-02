#include "Object.h"

#include "Intersection.h"
#include "Material.h"
#include "Window/UI.h"

namespace raytracing {

glm::vec3 Ray::At(float t) const { return origin + (direction * t); }

void Object::UI() {
    ImGui::Text(name.c_str());
    UI::SliderFloat3("Position", &position.x, -10.0f, 10.0f);
}

bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RaySphere(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Box::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayBox(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Plane::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayPlane(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Cylinder::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayCylinder(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Torus::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayTorus(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Rectangle::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayRectangle(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

void Rectangle::UI() {
    Object::UI();
    UI::SliderFloat3("Width", &width.x, -10.0f, 10.0f);
    UI::SliderFloat3("Height", &height.x, -10.0f, 10.0f);
}

}  // namespace raytracing