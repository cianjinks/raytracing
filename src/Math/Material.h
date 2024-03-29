#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Math/Math.h"
#include "Object.h"

namespace raytracing {

/* A material represents a scattering function for when a ray hits a surface. */
class Material {
   public:
    virtual bool scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const = 0;
    virtual glm::vec3 emitted() const { return glm::vec3(0.0f); }

    virtual void UI() {}
};

class Lambertian : public Material {
   public:
    glm::vec3 albedo;

    Lambertian(const glm::vec3 albedo) : albedo(albedo) {}
    virtual bool scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const override;
};

class Metal : public Material {
   public:
    glm::vec3 albedo;
    float fuzz;

    Metal(const glm::vec3 albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1.0f ? fuzz : 1.0f) {}
    virtual bool scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const override;
};

class Dielectric : public Material {
   public:
    float ior;

    Dielectric(float ior) : ior(ior) {}
    virtual bool scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const override;

   private:
    static float reflectance(float cosine, float ref_idx);
};

class DiffuseLight : public Material {
   public:
    glm::vec3 color;
    float intensity = 1.0f;

    DiffuseLight(const glm::vec3& c, float intensity) : color(c), intensity(intensity) {}
    virtual bool scatter(const Ray& ray, uint32_t& seed, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const override;
    virtual glm::vec3 emitted() const override;

    virtual void UI() override;
};

}  // namespace raytracing