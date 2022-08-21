#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImageView.h"

namespace raytracing {

class Window {
   public:
    std::string Name;
    int32_t Width;
    int32_t Height;

   private:
    GLFWwindow* m_Window;

   public:
    Window(std::string name, int32_t width, int32_t height);
    ~Window();

    void OnUpdate();

    bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
    GLFWwindow* GetGLFWWindow() const { return m_Window; }
};

}  // namespace raytracing