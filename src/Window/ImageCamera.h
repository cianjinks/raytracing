#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Event.h"

/**
 * @brief Camera
 * Camera is a 2D orthographic camera for panning and zooming around.
 */

namespace raytracing {

class ImageCamera {
   private:
    float m_Width;
    float m_Height;

    glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
    float m_Distance = 1.0f;
    float m_MaxDistance = 1.0f;
    float m_MinDistance = 0.05f;
    glm::vec2 m_InitialMousePosition;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

   public:
    ImageCamera(float width, float height);
    ~ImageCamera();

    void OnUpdate();
    void OnEvent(Event& event);
    void PollInput();

    void KeyPan(float x, float y);
    void MousePan();
    void Zoom(float delta);
    void Center(float distance);
    void Resize(float width, float height);

    glm::mat4 GetProjectionView() { return m_ProjectionMatrix * m_ViewMatrix; }

   private:
    void UpdateProjection();
    void UpdateView();
};

}  // namespace raytracing