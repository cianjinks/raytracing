#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

namespace raytracing {

class Ray {
   public:
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 At(float t) const;
};

class Material;

class HitResult {
   public:
    glm::vec3 position;
    glm::vec3 normal;
    S<Material> material;
    float t;
};

class Hittable {
   public:
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray& ray, float t_min, float t_max,
                     HitResult& hit) const = 0;
};

class Object : public Hittable {
   public:
    std::string name;
    glm::vec3 position;
    S<Material> material;

    Object(std::string name, glm::vec3 position, S<Material> material)
        : name(name), position(position), material(material) {}
    virtual ~Object() = default;
};

class Sphere : public Object {
   public:
    float radius;

    Sphere(std::string name, glm::vec3 position, S<Material> material, float radius)
        : Object(name, position, material), radius(radius) {}
    ~Sphere() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;
};

class Box : public Object {
   public:
    glm::vec3 size;

    Box(std::string name, glm::vec3 position, S<Material> material, glm::vec3 size)
        : Object(name, position, material), size(size) {}
    ~Box() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;
};

class Plane : public Object {
   public:
    glm::vec3 normal;

    /* Object::position will be treated as a point on the plane. */
    Plane(std::string name, glm::vec3 position, S<Material> material, glm::vec3 normal)
        : Object(name, position, material), normal(normal) {}
    ~Plane() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;
};

class Cylinder : public Object {
   public:
    glm::vec3 capA;
    glm::vec3 capB;
    float radius;

    Cylinder(std::string name, S<Material> material, glm::vec3 capA, glm::vec3 capB, float radius)
        : Object(name, {0, 0, 0}, material), capA(capA), capB(capB) {}
    ~Cylinder() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;
};

class Torus : public Object {
   public:
    float large_radius;
    float small_radius;

    Torus(std::string name, glm::vec3 position, S<Material> material, float large_radius, float small_radius)
        : Object(name, position, material),
          large_radius(large_radius),
          small_radius(small_radius) {}
    ~Torus() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;
};

}  // namespace raytracing