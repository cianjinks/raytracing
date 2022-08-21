#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Camera
 * Camera is a 2D orthographic camera for panning and zooming around.
 */

namespace raytracing {

class Camera {
   private:
    float m_Width;
    float m_Height;

    glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
    float m_Distance = 1.0f;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

   public:
    Camera(float width, float height);
    ~Camera();

    void OnUpdate();

    glm::mat4 GetProjectionView() { return m_ProjectionMatrix * m_ViewMatrix; }

   private:
    void UpdateProjection();
    void UpdateView();
};

}  // namespace raytracing