#pragma once

#include <glm/glm.hpp>

namespace raytracing {

class Ray {
   public:
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 At(float t) const;
};

class HitResult {
   public:
    glm::vec3 position;
    glm::vec3 normal;
    float t;
};

class Hittable {
   public:
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray& ray, float t_min, float t_max,
                     HitResult& hit) = 0;
};

class Scene : public Hittable {
   private:
    std::vector<Hittable*> m_Objects;

   public:
    Scene();
    ~Scene();

    void Add(Hittable* object);
    void Remove(Hittable* object);

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

class Sphere : public Hittable {
   public:
    glm::vec3 position;
    float radius;

    Sphere(glm::vec3 position, float radius)
        : position(position), radius(radius) {}
    ~Sphere() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

}  // namespace raytracing