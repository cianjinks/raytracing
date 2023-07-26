#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <nfd.h>

#include "Event.h"
#include "ImageView.h"

namespace raytracing {

class Window {
    using CallbackFunc = std::function<void(Event&)>;

   public:
    std::string Name;
    int32_t Width;
    int32_t Height;

   private:
    GLFWwindow* m_Window;
    CallbackFunc m_EventCallback;

    bool m_CursorCaptured = false;
    bool m_FirstMouse = true;

   public:
    Window(std::string name, int32_t width, int32_t height);
    ~Window();

    void OnUpdate();

    bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
    GLFWwindow* GetGLFWWindow() const { return m_Window; }
    void SetEventCallback(CallbackFunc callback) { m_EventCallback = callback; }

    void CaptureCursor();
    void ReleaseCursor();
    bool IsCursorCaptured() { return m_CursorCaptured; }

    void SetMouseStatus(bool status) { m_FirstMouse = status; }
    bool GetMouseStatus() { return m_FirstMouse; }
};

}  // namespace raytracing