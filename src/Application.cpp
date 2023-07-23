#include "Application.h"

#include <iostream>

#include "imgui.h"
namespace raytracing {

Window* Application::s_Window = nullptr;
ImageView* Application::s_ImageView = nullptr;

Application::Application(ApplicationInfo app_info) : AppInfo(app_info) {
    RT_PROFILE_FUNC_N("Application Init");

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
    m_DeviceManager = new RenderDeviceManager(s_ImageView->GetTexture());
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
    RT_PROFILE_FUNC;

    while (!s_Window->ShouldClose()) {
        m_DeviceManager->OnUpdate();
        s_ImageView->OnUpdate();
        UI::PreRender();
        UI();
        UI::Render();
        s_Window->OnUpdate();
    }
}

void Application::UI() {
    RT_PROFILE_FUNC;

    ImGui::Begin("Controls");
    m_DeviceManager->UI();
    s_ImageView->UI();
    ImGui::End();
}

void Application::OnEvent(Event& event) {
    RT_PROFILE_FUNC;

    if (UI::WantInput()) {
        return;
    }
    s_ImageView->OnEvent(event);
}

}  // namespace raytracing