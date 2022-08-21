#include "Camera.h"

namespace raytracing {

Camera::Camera(float width, float height) : m_Width(width), m_Height(height) {
    UpdateProjection();
    UpdateView();
}

Camera::~Camera() {}

void Camera::OnUpdate() { UpdateView(); }

void Camera::UpdateProjection() {
    m_ProjectionMatrix =
        glm::ortho(-(m_Width / 2.0f), (m_Width / 2.0f), -(m_Height / 2.0f),
                   (m_Height / 2.0f), -1.0f, 1.0f);
}

void Camera::UpdateView() {
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));
    m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(0.3f));
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

}  // namespace raytracing