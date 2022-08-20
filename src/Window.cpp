#include "Window.h"

namespace raytracing {

Window::Window(std::string name, int32_t width, int32_t height)
    : Name(name), Width(width), Height(height) {
    /* Initialize the library */
    if (!glfwInit()) {
        /* TODO: Assert! */
    }

    m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        /* TODO: Assert! */
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        /* TODO: Assert! */
    }
}

Window::~Window() { glfwTerminate(); }

void Window::OnUpdate() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

}  // namespace raytracing