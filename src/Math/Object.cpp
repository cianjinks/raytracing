#include "Object.h"

#include "Intersection.h"
#include "Material.h"
#include "Window/UI.h"

namespace raytracing {

glm::vec3 Ray::At(float t) const { return origin + (direction * t); }

Transform::Transform(S<Hittable> hittable) : m_Hittable(hittable) {
    Recalculate();
}

Transform::Transform(S<Hittable> hittable, const glm::vec3& object_position) : m_Hittable(hittable), m_ObjectPosition(object_position) {
    Recalculate();
}

void Transform::Recalculate() {
    m_TransformWS = glm::mat4(1.0f);

    /* To position objects they have a `position` variable. This needs to be accounted for so there is a special constructor. */
    /* I don't want to have to wrap every object with a Transform node, but I suppose in a DAG that's how it is normally done. */
    /* At the moment object->position is much more efficient than wrapping everything in Transform. */
    m_TransformWS = glm::translate(m_TransformWS, m_Translation + m_ObjectPosition);
    m_TransformWS = glm::rotate(m_TransformWS, glm::radians(m_RotationAngle), m_RotationAxis);
    m_TransformWS = glm::scale(m_TransformWS, m_Scale);
    m_TransformWS = glm::translate(m_TransformWS, -m_ObjectPosition);

    m_TransformOS = glm::inverse(m_TransformWS);
}

bool Transform::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    Ray object_space_ray;
    object_space_ray.origin = m_TransformOS * glm::vec4(ray.origin, 1.0f);
    object_space_ray.direction = m_TransformOS * glm::vec4(ray.direction, 0.0f);

    if (!m_Hittable->Hit(object_space_ray, seed, t_min, t_max, hit)) {
        return false;
    }

    hit.position = m_TransformWS * glm::vec4(hit.position, 1.0f);
    hit.normal = glm::transpose(m_TransformOS) * glm::vec4(hit.normal, 1.0f);  // TODO: Is this correct?
    hit.normal = glm::normalize(hit.normal);

    return true;
}

void Object::UI() {
    ImGui::Text("%s", name.c_str());
    UI::SliderFloat3("Position", &position.x, -10.0f, 10.0f);
}

bool Sphere::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RaySphere(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Box::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayBox(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Plane::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayPlane(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Cylinder::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayCylinder(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Torus::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    if (Intersection::RayTorus(ray, *this, t_min, t_max, hit)) {
        hit.material = material;
        return true;
    }
    return false;
}

bool Rectangle::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
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

bool ConstantMedium::Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const {
    // TODO: Support non-convex objects
    HitResult h1, h2;

    // Record first object hit (entrance)
    // Go from FLowest to FMax to make it work even if we're inside the object.
    // It will find the entrance even if it's behind the camera.
    if (!object->Hit(ray, seed, Constant::FLowest, Constant::FMax, h1)) {
        return false;
    }
    // Record next object hit (exit, if convex)
    // Always beyond the previous hit
    if (!object->Hit(ray, seed, h1.t + 0.0001f, Constant::FInfinity, h2)) {
        return false;
    }

    // Clamp t values
    if (h1.t < t_min) {
        h1.t = t_min;
    }
    if (h2.t > t_max) {
        h2.t = t_max;
    }

    // Safety Checks
    if (h1.t >= h2.t) { return false; }
    if (h1.t < 0.0f) { h1.t = 0.0f; }

    // TODO: Why is this not always 1 if ray direction is normalized?
    float ray_length = glm::length(ray.direction);
    float dist_in_object = (h2.t - h1.t) * ray_length;
    float hit_dist = negative_inverse_density * glm::log(FastRandom::Float(seed));
    if (hit_dist > dist_in_object) {
        return false;
    }

    hit.t = h1.t + (hit_dist / ray_length);
    hit.position = ray.At(hit.t);
    hit.material = object->material;
    // unused
    hit.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    hit.uv = glm::vec2(0.0f);

    return true;
}

}  // namespace raytracing
