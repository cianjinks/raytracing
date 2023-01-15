#include "Material.h"

namespace raytracing {

bool Lambertian::scatter(const Ray& ray, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const {
    Ray bounce_ray;
    bounce_ray.origin = hit.position;

    /* 2 different formulas for diffuse reflection. */
    // glm::vec3 target = Random::InHemisphere(result.normal);
    glm::vec3 target = hit.normal + glm::normalize(Random::InSphere());

    if (Math::V3NearZero(target))
        target = hit.normal;
    bounce_ray.direction = target;
    attenuation = albedo;
    return true;
}

}  // namespace raytracing