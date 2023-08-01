#pragma once

#include <glm/glm.hpp>

#include "Window/Event.h"

namespace raytracing {

/* Camera doesn't need to be object / hittable. */
class Camera {
   public:
    /* Positional */
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    /* Lens */
    bool useLens = false;
    float vfov = 90.0f;
    float aperture = 2.0f;
    float focus_dist = 2.5f;

    /* Controls */
    float speed = 0.2f;
    float yaw = 0.0f;    // radians
    float pitch = 0.0f;  // radians
    float lastX = 0.0f;
    float lastY = 0.0f;

    Camera() {}
    Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float speed, float vfov, float aperture, float focus_dist);
    ~Camera() = default;

    inline void SetPosition(const glm::vec3& pos) { position = pos; }
    inline void SetDirection(const glm::vec3& dir) {
        direction = glm::normalize(dir);
        UpdatePY();
    }

    bool OnUpdate();
    void OnEvent(Event& event);

   private:
    void UpdatePY() {
        pitch = glm::asin(direction.y);
        yaw = glm::atan(direction.z, direction.x);
    }
};

}  // namespace raytracing