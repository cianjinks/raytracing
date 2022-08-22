#include "Application.h"

#include <iostream>

#include "imgui.h"
namespace raytracing {

Window* Application::s_Window = nullptr;
ImageView* Application::s_ImageView = nullptr;

Application::Application(ApplicationInfo app_info) : AppInfo(app_info) {
    Log::Init();
    if (!s_Window) {
        s_Window = new Window(AppInfo.Name.c_str(), AppInfo.WindowWidth,
                              AppInfo.WindowHeight);
        s_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
    }
    if (!s_ImageView) {
        s_ImageView = new ImageView(AppInfo.WindowWidth, AppInfo.WindowHeight,
                                    AppInfo.ImageWidth, AppInfo.ImageHeight);
    }
    m_DeviceManager = new RenderDeviceManager();
    UI::Init();
    RT_LOG("Application Initialised");
}

Application::~Application() {
    delete s_ImageView;
    s_ImageView = nullptr;
    delete s_Window;
    s_Window = nullptr;
    delete m_DeviceManager;
    m_DeviceManager = nullptr;
}

void Application::Run() {
    while (!s_Window->ShouldClose()) {
        s_ImageView->OnUpdate();

        UI::PreRender();
        UI();
        UI::Render();

        s_Window->OnUpdate();
    }
}

void Application::UI() {
    ImGui::Begin("Controls");
    m_DeviceManager->UI();
    s_ImageView->UI();
    ImGui::End();
}

void Application::OnEvent(Event& event) {
    if (UI::WantInput()) {
        return;
    }
    s_ImageView->OnEvent(event);
}

}  // namespace raytracing