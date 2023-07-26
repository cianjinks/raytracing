#include "Camera.h"

#include "Window/Input.h"

namespace raytracing {

bool Camera::OnUpdate() {
    bool right_click = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);
    if (right_click && !Application::GetWindow()->IsCursorCaptured()) {
        Application::GetWindow()->CaptureCursor();
    } else if (!right_click) {
        Application::GetWindow()->ReleaseCursor();
        Application::GetWindow()->SetMouseStatus(true);
    }

    bool change = false;
    if (Input::IsKeyPressed(GLFW_KEY_W)) {
        position += speed * glm::normalize(direction);
        change = true;
    }
    if (Input::IsKeyPressed(GLFW_KEY_S)) {
        position -= speed * glm::normalize(direction);
        change = true;
    }
    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        position -= glm::normalize(glm::cross(direction, up)) * speed;
        change = true;
    }
    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        position += glm::normalize(glm::cross(direction, up)) * speed;
        change = true;
    }

    return change;
}

void Camera::OnEvent(Event& event) {
    if (!Application::GetWindow()->IsCursorCaptured()) {
        return;
    }

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MousePositionEvent>(EventType::MOUSE_POSITION, [this](auto& event) {
        double mouseX = event.GetXPos();
        double mouseY = event.GetYPos();

        if (Application::GetWindow()->GetMouseStatus()) {
            lastX = (float)mouseX;
            lastY = (float)mouseY;
            Application::GetWindow()->SetMouseStatus(false);
        }

        float xoffset = (float)mouseX - lastX;
        float yoffset = lastY - (float)mouseY;
        lastX = (float)mouseX;
        lastY = (float)mouseY;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(direction);
    });
}

}  // namespace raytracing