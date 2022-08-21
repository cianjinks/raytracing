#include "Input.h"

namespace raytracing {

bool Input::IsKeyPressed(int key) {
    GLFWwindow* window = Application::GetWindow()->GetGLFWWindow();
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button) {
    GLFWwindow* window = Application::GetWindow()->GetGLFWWindow();
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

glm::dvec2 Input::GetMouseCoord() {
    GLFWwindow* window = Application::GetWindow()->GetGLFWWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {xpos, ypos};
}

double Input::GetMouseX() { return GetMouseCoord().x; }

double Input::GetMouseY() { return GetMouseCoord().y; }

}  // namespace raytracing