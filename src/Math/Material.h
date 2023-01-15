#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Math/Math.h"
#include "Object.h"

namespace raytracing {

class Material {
   public:
    virtual bool scatter(const Ray& ray, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
   public:
    glm::vec3 albedo;

    Lambertian(const glm::vec3 albedo) : albedo(albedo) {}
    virtual bool scatter(const Ray& ray, const HitResult& hit, glm::vec3& attenuation, Ray& scattered) const override;
};

}  // namespace raytracing