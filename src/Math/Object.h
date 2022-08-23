#pragma once

#include <glm/glm.hpp>

namespace raytracing {

class Camera {
   public:
    glm::vec3 position;
    glm::vec3 direction;

    Camera(glm::vec3 position, glm::vec3 direction)
        : position(position), direction(direction) {}
    ~Camera() = default;
};

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

}  // namespace raytracing