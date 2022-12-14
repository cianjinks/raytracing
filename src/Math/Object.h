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

class Object : public Hittable {
   public:
    std::string name;
    glm::vec3 position;

    Object(std::string name, glm::vec3 position)
        : name(name), position(position) {}
    virtual ~Object() = default;
};

/* Camera doesn't need to be object / hittable. */
class Camera {
   public:
    glm::vec3 position;
    glm::vec3 direction;

    Camera(glm::vec3 position, glm::vec3 direction)
        : position(position), direction(direction) {}
    ~Camera() = default;
};

class Scene : public Object {
   private:
    std::vector<Object*> m_Objects;

   public:
    Scene(std::string name, glm::vec3 position);
    ~Scene();

    void Add(Object* object);
    void Remove(Object* object);

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;

    std::vector<Object*>& GetObjects() { return m_Objects; }
};

class Sphere : public Object {
   public:
    float radius;

    Sphere(std::string name, glm::vec3 position, float radius)
        : Object(name, position), radius(radius) {}
    ~Sphere() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

class Box : public Object {
   public:
    glm::vec3 size;

    Box(std::string name, glm::vec3 position, glm::vec3 size)
        : Object(name, position), size(size) {}
    ~Box() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

class Plane : public Object {
   public:
    glm::vec3 normal;

    /* Object::position will be treated as a point on the plane. */
    Plane(std::string name, glm::vec3 position, glm::vec3 normal)
        : Object(name, position), normal(normal) {}
    ~Plane() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

class Cylinder : public Object {
   public:
    glm::vec3 capA;
    glm::vec3 capB;
    float radius;

    Cylinder(std::string name, glm::vec3 capA, glm::vec3 capB, float radius)
        : Object(name, {0, 0, 0}), capA(capA), capB(capB) {}
    ~Cylinder() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

class Torus : public Object {
   public:
    float large_radius;
    float small_radius;

    Torus(std::string name, glm::vec3 position, float large_radius,
          float small_radius)
        : Object(name, position),
          large_radius(large_radius),
          small_radius(small_radius) {}
    ~Torus() = default;

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) override;
};

}  // namespace raytracing