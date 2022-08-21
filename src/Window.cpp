#include "Window.h"

namespace raytracing {

Window::Window(std::string name, int32_t width, int32_t height)
    : Name(name), Width(width), Height(height) {
    /* Initialize the library */
    if (!glfwInit()) {
        RT_ASSERT(false, "Failed to initialise GLFW!");
    }

    m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        RT_ASSERT(false, "Failed to initialise GLFW window!");
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        RT_ASSERT(false, "Failed to initialise GLAD!");
    }
}

Window::~Window() { glfwTerminate(); }

void Window::OnUpdate() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

}  // namespace raytracing