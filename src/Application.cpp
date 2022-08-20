#include "Application.h"

#include <iostream>

#include "imgui.h"

namespace raytracing {

Window* Application::s_Window = nullptr;

Application::Application() {
    if (!s_Window) {
        s_Window = new Window("Raytracing", 1280, 720);
    }

    UI::Init();
}

Application::~Application() {
    delete s_Window;
    s_Window = nullptr;
}

void Application::Run() {
    while (!s_Window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

        /* UI */
        UI::PreRender();
        ImGui::ShowDemoWindow();
        UI::Render();

        s_Window->OnUpdate();
    }
}

}  // namespace raytracing