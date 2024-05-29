#include "Material.h"

#include "Window/UI.h"

namespace raytracing {

bool Lambertian::scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const {
    RT_PROFILE_FUNC;

    scattered.origin = hit.position;

    /* 2 different formulas for diffuse reflection. */
    glm::vec3 target = FastRandom::InHemisphere(seed, hit.normal);
    // glm::vec3 target = hit.normal + glm::normalize(FastRandom::InSphere(seed));

    if (Math::V3NearZero(target)) {
        target = hit.normal;
    }

    scattered.direction = target;
    attenuation = texture->sample(hit.uv.x, hit.uv.y, hit.position);
    return true;
}

/* Just reflect across normal. */
bool Metal::scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const {
    RT_PROFILE_FUNC;

    scattered.origin = hit.position;
    scattered.direction = Math::V3Reflect(glm::normalize(ray.direction), hit.normal) + (fuzz * FastRandom::InSphere(seed));
    attenuation = albedo;
    return glm::dot(scattered.direction, hit.normal) > 0;
}

bool Dielectric::scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const {
    RT_PROFILE_FUNC;

    attenuation = glm::vec3(1.0f);

    bool face_test = glm::dot(ray.direction, hit.normal) < 0;
    float refraction_ratio = face_test ? (1.0f / ior) : ior;

    glm::vec3 norm_direction = glm::normalize(ray.direction);
    float cos_theta = glm::min(glm::dot(-norm_direction, hit.normal), 1.0f);
    float sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0f;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > FastRandom::Float(seed)) {
        scattered.direction = Math::V3Reflect(norm_direction, hit.normal);
    } else {
        scattered.direction = Math::V3Refract(norm_direction, hit.normal, refraction_ratio);
    }

    scattered.origin = hit.position;
    return true;
}

float Dielectric::reflectance(float cosine, float ref_idx) {
    float r0 = (1.0f - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * glm::pow(1.0f - cosine, 5.0f);
}

bool Isotropic::scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const {
    RT_PROFILE_FUNC;
    
    scattered.origin = hit.position;
    scattered.direction = FastRandom::Vec3(seed);
    attenuation = texture->sample(hit.uv.x, hit.uv.y, hit.position);
    return true;
}

bool DiffuseLight::scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const {
    return false;
}

glm::vec3 DiffuseLight::emitted() const {
    return intensity * color;
}

void DiffuseLight::UI() {
    ImGui::Text("Diffuse Material");
    UI::ColorEdit3("Color", &color.r);
    UI::SliderFloat("Intensity", &intensity, 0.0f, 50.0f);
}

}  // namespace raytracing