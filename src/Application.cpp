#include "Application.h"

#include <iostream>

#include "imgui.h"

namespace raytracing {

Window* Application::s_Window = nullptr;

Application::Application() {
    Log::Init();
    if (!s_Window) {
        s_Window = new Window("Raytracing", 1280, 720);
    }
    UI::Init();
    RT_LOG("Application Initialised");
}

Application::~Application() {
    delete s_Window;
    s_Window = nullptr;
}

void Application::Run() {
    while (!s_Window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

        /* UI */
        UI::PreRender();
        ImGui::ShowDemoWindow();
        UI::Render();

        s_Window->OnUpdate();
    }
}

}  // namespace raytracing