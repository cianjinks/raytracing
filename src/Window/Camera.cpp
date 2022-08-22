#include "Camera.h"

#include "Input.h"

namespace raytracing {

Camera::Camera(float width, float height)
    : m_Width(width),
      m_Height(height),
      m_InitialMousePosition(width / 2.0f, height / 2.0f) {
    UpdateProjection();
    UpdateView();
}

Camera::~Camera() {}

void Camera::OnUpdate() {
    UpdateView();
    PollInput();
}

void Camera::OnEvent(Event &event) {
    EventType type = event.GetEventType();
    switch (type) {
        case EventType::WINDOW_RESIZE: {
            WindowResizeEvent &resize_event =
                static_cast<WindowResizeEvent &>(event);
            Resize((float)resize_event.GetWidth(),
                   (float)resize_event.GetHeight());
            break;
        }
        case EventType::MOUSE_SCROLL: {
            MouseScrollEvent &scroll_event =
                static_cast<MouseScrollEvent &>(event);
            Zoom(scroll_event.GetYOffset());
            break;
        }
        case EventType::MOUSE_PRESS: {
            MouseButtonEvent &mouse_event =
                static_cast<MouseButtonEvent &>(event);
            int but = mouse_event.GetButton();
            if (but == GLFW_MOUSE_BUTTON_MIDDLE) {
                m_InitialMousePosition =
                    glm::vec2(Input::GetMouseX(), Input::GetMouseY());
            }
            break;
        }
    }
}

void Camera::PollInput() {
    float panSpeed = 5.0f * m_Distance;
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        MousePan();
    }
    if (Input::IsKeyPressed(GLFW_KEY_W)) {
        KeyPan(0.0f, panSpeed);
    }
    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        KeyPan(-panSpeed, 0.0f);
    }
    if (Input::IsKeyPressed(GLFW_KEY_S)) {
        KeyPan(0.0f, -panSpeed);
    }
    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        KeyPan(panSpeed, 0.0f);
    }
}

void Camera::UpdateProjection() {
    m_ProjectionMatrix =
        glm::ortho(-(m_Width / 2.0f), (m_Width / 2.0f), -(m_Height / 2.0f),
                   (m_Height / 2.0f), -1.0f, 1.0f);
}

void Camera::UpdateView() {
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));
    m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(m_Distance));
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

void Camera::KeyPan(float x, float y) { m_Position += glm::vec2(x, y); }

void Camera::MousePan() {
    glm::vec2 mouse = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
    glm::vec2 delta = (mouse - m_InitialMousePosition) * m_Distance;
    m_InitialMousePosition = mouse;
    m_Position.x -= delta.x;
    m_Position.y += delta.y;
}

void Camera::Zoom(float delta) {
    float speed = 0.35f * (m_Distance * m_Distance);
    m_Distance -= delta * speed;
    m_Distance = std::min(m_Distance, m_MaxDistance);
    m_Distance = std::max(m_Distance, m_MinDistance);
}

void Camera::Center(float distance) {
    m_Position.x = 0.0f;
    m_Position.y = 0.0f;
    m_Distance = distance;
}

void Camera::Resize(float width, float height) {
    m_Width = width;
    m_Height = height;
    UpdateProjection();
}

}  // namespace raytracing