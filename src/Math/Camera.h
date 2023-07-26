#pragma once

#include <glm/glm.hpp>

#include "Window/Event.h"

namespace raytracing {

/* Camera doesn't need to be object / hittable. */
class Camera {
   public:
    /* Positional */
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    /* Lens */
    float vfov;
    float aperture;
    float focus_dist;

    /* Controls */
    float speed;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float lastX = 0.0f;
    float lastY = 0.0f;

    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float speed, float vfov, float aperture, float focus_dist)
        : position(position), direction(direction), up(up), speed(speed), vfov(vfov), aperture(aperture), focus_dist(focus_dist) {}
    ~Camera() = default;

    bool OnUpdate();
    void OnEvent(Event& event);
};

}  // namespace raytracing