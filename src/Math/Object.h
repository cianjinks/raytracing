#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "BBox.h"
#include "Ray.h"

namespace raytracing {

class Material;

class HitResult {
   public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    S<Material> material;
    float t;
};

class Hittable {
   public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max,
                     HitResult& hit) const = 0;

    virtual BBox BoundingBox() const = 0;
};

class Object : public Hittable {
   public:
    std::string name;
    glm::vec3 position;
    S<Material> material;

    Object(std::string name, glm::vec3 position, S<Material> material)
        : name(name), position(position), material(material) {}
    virtual ~Object() = default;

    virtual void UI();

    BBox BoundingBox() const override { return bbox; }

   protected:
    BBox bbox; /* All objects have a bounding box. Empty by default. */
};

class Transform : public Hittable {
   public:
    Transform(S<Object> object) : object(object) {}
    virtual ~Transform() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;

    BBox BoundingBox() const override { return object->BoundingBox(); }

   public:
    glm::vec3 translation = glm::vec3(0.0f);
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

   private:
    S<Object> object;
};

class Sphere : public Object {
   public:
    float radius;

    Sphere(std::string name, glm::vec3 position, S<Material> material, float radius)
        : Object(name, position, material), radius(radius) {
        bbox = BBox(position - glm::vec3(radius), position + glm::vec3(radius));
    }
    ~Sphere() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;
};

class Box : public Object {
   public:
    glm::vec3 size;

    Box(std::string name, glm::vec3 position, S<Material> material, glm::vec3 size)
        : Object(name, position, material), size(size) {
        /* This is what Intersection::RayBox does to determine extents. TODO: change */
        bbox = BBox(position - size, position + size);
    }
    ~Box() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;
};

class Plane : public Object {
   public:
    glm::vec3 normal;

    /* Object::position will be treated as a point on the plane. */
    Plane(std::string name, glm::vec3 position, S<Material> material, glm::vec3 normal)
        : Object(name, position, material), normal(normal) {
        /* Note: Bounding box does not make sense for an infinite plane. */
    }
    ~Plane() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;
};

class Cylinder : public Object {
   public:
    glm::vec3 capA;
    glm::vec3 capB;
    float radius;

    Cylinder(std::string name, S<Material> material, glm::vec3 capA, glm::vec3 capB, float radius)
        : Object(name, {0, 0, 0}, material), capA(capA), capB(capB) {
        /* TODO: Bounding box. */
    }
    ~Cylinder() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;
};

class Torus : public Object {
   public:
    float large_radius;
    float small_radius;

    Torus(std::string name, glm::vec3 position, S<Material> material, float large_radius, float small_radius)
        : Object(name, position, material),
          large_radius(large_radius),
          small_radius(small_radius) {
        /* TODO: Bounding box. */
    }
    ~Torus() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;
};

class Rectangle : public Object {
   public:
    /* Rectangle position represents the bottom left corner. */
    /* Rectangle width and height are vectors relative to that corner, representing the bounds of the rectangle. */
    glm::vec3 width;
    glm::vec3 height;

    Rectangle(std::string name, glm::vec3 position, S<Material> material, glm::vec3 width, glm::vec3 height)
        : Object(name, position, material), width(width), height(height) {
        /* TODO: Verify this is correct. */
        bbox = BBox(position, position + width + height);
        bbox.Pad(); /* Rect will have a zero axis bounding box. */
    }
    ~Rectangle() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;

    virtual void UI() override;
};

class ConstantMedium : public Hittable {
   public:
    // TODO: Does material always need to be isotropic?
    ConstantMedium(S<Object> object, float density) 
        : object(object), negative_inverse_density(-1.0f / density) {}
    virtual ~ConstantMedium() = default;

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;

    BBox BoundingBox() const override { return object->BoundingBox(); }

   public:
    float negative_inverse_density = 0.0f;

   private:
    S<Object> object;
};

}  // namespace raytracing