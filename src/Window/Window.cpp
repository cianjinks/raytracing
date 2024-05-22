#include "Window.h"

namespace raytracing {

Window::Window(std::string name, int32_t width, int32_t height)
    : Name(name), Width(width), Height(height) {
    RT_PROFILE_FUNC_N("Window Init");

    if (!glfwInit()) {
        RT_ASSERT(false, "Failed to initialise GLFW!");
    }

    glfwSetErrorCallback([](int error, const char* msg) {
        RT_ERROR("[GLFW Error] [{0}] {1}", error, msg);
    });

    m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        RT_ASSERT(false, "Failed to initialise GLFW window!");
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        RT_ASSERT(false, "Failed to initialise GLAD!");
    }

    if (NFD_Init() != NFD_OKAY) {
        RT_ASSERT(false, "Failed to initialise Native File Dialog!");
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

Window::~Window() {
    NFD_Quit();
    glfwTerminate();
}

void Window::OnUpdate() {
    RT_PROFILE_FUNC_N("Window Swap");
    glfwSwapBuffers(m_Window);
    RT_PROFILE_GPU_SWAP;
    glfwPollEvents();
}

void Window::CaptureCursor() {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_CursorCaptured = true;
}

void Window::ReleaseCursor() {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_CursorCaptured = false;
}

float Window::GetContentScale() {
    float xscale = 1.0f;
    glfwGetWindowContentScale(m_Window, &xscale, nullptr);
    return xscale;
}

}  // namespace raytracing