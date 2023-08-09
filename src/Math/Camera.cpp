#include "Camera.h"

#include "Window/Input.h"

namespace raytracing {

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float speed, float vfov, float aperture, float focus_dist)
    : position(pos), direction(dir), up(up), speed(speed), vfov(vfov), aperture(aperture), focus_dist(focus_dist) {
    SetDirection(direction);
}

Ray Camera::GetRay(float s, float t, float aspect_ratio, uint32_t seed) const {
    float view_plane_height = 2.0f * glm::tan(glm::radians(vfov) / 2.0f);
    float view_plane_width = aspect_ratio * view_plane_height;

    glm::vec3 u = glm::cross(glm::normalize(direction), up);
    glm::vec3 v = glm::cross(u, glm::normalize(direction));

    glm::vec3 vertical = view_plane_height * v;
    glm::vec3 horizontal = view_plane_width * u;

    /* Camera lens calculations. */
    glm::vec3 cd = glm::normalize(direction);
    glm::vec3 offset = glm::vec3(0.0f);
    if (useLens) {
        /* Offset camera position by point in a random disk oriented along camera view plane. */
        /* This simulates shooting rays from a circular lens. */
        /* TODO: My aperture seems to be off by a factor of 2. Maybe this divide by 2 is wrong because of -1 to 1 range? */
        glm::vec3 random_disk = (aperture / 2.0f) * FastRandom::InUnitDisk(seed);
        offset = (u * random_disk.x) + (v * random_disk.y);

        /* Scale camera direction by focus_dist to push view plane farther away. */
        /* We also need to scale vertical / horizontal otherwise the view will be zoomed. */
        vertical = focus_dist * vertical;
        horizontal = focus_dist * horizontal;
        cd = focus_dist * cd;
    }

    Ray ray;
    ray.origin = position + offset;
    /* Half vertical and horizontal because camera.direction is in the centre of the view plane, but they were calculated with the full width and height. */
    ray.direction = cd + ((horizontal / 2.0f) * s) + ((vertical / 2.0f) * t) - offset;

    return ray;
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