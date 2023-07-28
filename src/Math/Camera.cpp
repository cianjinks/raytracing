#include "Camera.h"

#include "Window/Input.h"

namespace raytracing {

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float speed, float vfov, float aperture, float focus_dist)
    : position(pos), direction(dir), up(up), speed(speed), vfov(vfov), aperture(aperture), focus_dist(focus_dist) {
    direction = glm::normalize(direction);
    pitch = glm::asin(direction.y);
    yaw = glm::atan(direction.z, direction.x);
}

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

        yaw += glm::radians(xoffset);
        pitch += glm::radians(yoffset);

        if (pitch > glm::radians(89.0f))
            pitch = glm::radians(89.0f);
        if (pitch < glm::radians(-89.0f))
            pitch = glm::radians(-89.0f);

        direction.x = glm::cos(yaw) * glm::cos(pitch);
        direction.y = glm::sin(pitch);
        direction.z = glm::sin(yaw) * glm::cos(pitch);
        direction = glm::normalize(direction);
    });
}

}  // namespace raytracing