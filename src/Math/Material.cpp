#include "Material.h"

namespace raytracing {

bool Lambertian::scatter(const Ray& ray, const HitResult& hit,
                         glm::vec3& attenuation, Ray& scattered) const {
    scattered.origin = hit.position;

    /* 2 different formulas for diffuse reflection. */
    // glm::vec3 target = Random::InHemisphere(result.normal);
    glm::vec3 target = hit.normal + glm::normalize(Random::InSphere());

    if (Math::V3NearZero(target)) target = hit.normal;
    scattered.direction = target;
    attenuation = albedo;
    return true;
}

/* Just reflect across normal. */
bool Metal::scatter(const Ray& ray, const HitResult& hit,
                    glm::vec3& attenuation, Ray& scattered) const {
    scattered.origin = hit.position;
    scattered.direction =
        Math::V3Reflect(glm::normalize(ray.direction), hit.normal);
    attenuation = albedo;
    return glm::dot(scattered.direction, hit.normal) > 0;
}

}  // namespace raytracing