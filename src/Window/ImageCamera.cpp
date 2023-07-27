#include "ImageCamera.h"

#include "Input.h"

namespace raytracing {

ImageCamera::ImageCamera(float width, float height)
    : m_Width(width),
      m_Height(height),
      m_InitialMousePosition(width / 2.0f, height / 2.0f) {
    UpdateProjection();
    UpdateView();
}

ImageCamera::~ImageCamera() {}

void ImageCamera::OnUpdate() {
    UpdateView();
    PollInput();
}

void ImageCamera::OnEvent(Event &event) {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<WindowResizeEvent>(
        EventType::WINDOW_RESIZE, [this](auto &event) {
            Resize((float)event.GetWidth(), (float)event.GetHeight());
        },
        false);

    dispatcher.Dispatch<MouseScrollEvent>(
        EventType::MOUSE_SCROLL, [this](auto &event) {
            Zoom(event.GetYOffset());
        },
        false);

    dispatcher.Dispatch<MouseButtonEvent>(
        EventType::MOUSE_PRESS, [this](auto &event) {
            int but = event.GetButton();
            if (but == GLFW_MOUSE_BUTTON_MIDDLE) {
                m_InitialMousePosition =
                    glm::vec2(Input::GetMouseX(), Input::GetMouseY());
            }
        },
        false);
}

void ImageCamera::PollInput() {
    float panSpeed = 5.0f * m_Distance;
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        MousePan();
    }

    // if (Input::IsKeyPressed(GLFW_KEY_W)) {
    //     KeyPan(0.0f, panSpeed);
    // }
    // if (Input::IsKeyPressed(GLFW_KEY_A)) {
    //     KeyPan(-panSpeed, 0.0f);
    // }
    // if (Input::IsKeyPressed(GLFW_KEY_S)) {
    //     KeyPan(0.0f, -panSpeed);
    // }
    // if (Input::IsKeyPressed(GLFW_KEY_D)) {
    //     KeyPan(panSpeed, 0.0f);
    // }
}

void ImageCamera::UpdateProjection() {
    m_ProjectionMatrix =
        glm::ortho(-(m_Width / 2.0f), (m_Width / 2.0f), -(m_Height / 2.0f),
                   (m_Height / 2.0f), -1.0f, 1.0f);
}

void ImageCamera::UpdateView() {
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));
    m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(m_Distance));
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

void ImageCamera::KeyPan(float x, float y) { m_Position += glm::vec2(x, y); }

void ImageCamera::MousePan() {
    glm::vec2 mouse = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
    glm::vec2 delta = (mouse - m_InitialMousePosition) * m_Distance;
    m_InitialMousePosition = mouse;
    m_Position.x -= delta.x;
    m_Position.y += delta.y;
}

void ImageCamera::Zoom(float delta) {
    float speed = 0.35f * (m_Distance * m_Distance);
    m_Distance -= delta * speed;
    m_Distance = std::min(m_Distance, m_MaxDistance);
    m_Distance = std::max(m_Distance, m_MinDistance);
}

void ImageCamera::Center(float distance) {
    m_Position.x = 0.0f;
    m_Position.y = 0.0f;
    m_Distance = distance;
    m_Distance = std::min(m_Distance, m_MaxDistance);
    m_Distance = std::max(m_Distance, m_MinDistance);
}

void ImageCamera::Resize(float width, float height) {
    m_Width = width;
    m_Height = height;
    UpdateProjection();
}

}  // namespace raytracing