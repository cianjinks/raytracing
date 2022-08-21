#include "Window.h"

namespace raytracing {

Window::Window(std::string name, int32_t width, int32_t height)
    : Name(name), Width(width), Height(height) {
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

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(m_Window, &m_EventCallback);

    glfwSetFramebufferSizeCallback(
        m_Window, [](GLFWwindow* window, int width, int height) {
            CallbackFunc fn =
                *static_cast<CallbackFunc*>(glfwGetWindowUserPointer(window));
            if (!fn) {
                RT_ASSERT(false, "No Event Callback function is set");
            }

            WindowResizeEvent event(width, height);
            fn(event);
        });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode,
                                    int action, int mods) {
        CallbackFunc fn =
            *static_cast<CallbackFunc*>(glfwGetWindowUserPointer(window));
        if (!fn) {
            RT_ASSERT(false, "No Event Callback function is set");
        }

        switch (action) {
            case GLFW_PRESS: {
                KeyEvent event(EventType::KEY_PRESS, key);
                fn(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyEvent event(EventType::KEY_RELEASE, key);
                fn(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyEvent event(EventType::KEY_REPEAT, key);
                fn(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(
        m_Window, [](GLFWwindow* window, double xpos, double ypos) {
            CallbackFunc fn =
                *static_cast<CallbackFunc*>(glfwGetWindowUserPointer(window));
            if (!fn) {
                RT_ASSERT(false, "No Event Callback function is set");
            }

            MousePositionEvent event(xpos, ypos);
            fn(event);
        });

    glfwSetMouseButtonCallback(
        m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            CallbackFunc fn =
                *static_cast<CallbackFunc*>(glfwGetWindowUserPointer(window));
            if (!fn) {
                RT_ASSERT(false, "No Event Callback function is set");
            }

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonEvent event(EventType::MOUSE_PRESS, button);
                    fn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonEvent event(EventType::MOUSE_RELEASE, button);
                    fn(event);
                    break;
                }
            }
        });

    glfwSetScrollCallback(
        m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            CallbackFunc fn =
                *static_cast<CallbackFunc*>(glfwGetWindowUserPointer(window));
            if (!fn) {
                RT_ASSERT(false, "No Event Callback function is set");
            }

            MouseScrollEvent event(xOffset, yOffset);
            fn(event);
        });

    RT_LOG("Window Initialised");
}

Window::~Window() { glfwTerminate(); }

void Window::OnUpdate() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

}  // namespace raytracing