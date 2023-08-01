#include "Application.h"

namespace raytracing {

S<Window> Application::s_Window = nullptr;
S<ImageView> Application::s_ImageView = nullptr;
bool Application::s_DisableUI = false;

Application::Application(ApplicationInfo app_info) : AppInfo(app_info) {
    RT_PROFILE_FUNC_N("Application Init");

    Log::Init();
    if (!s_Window) {
        s_Window = CreateS<Window>(AppInfo.Name.c_str(), AppInfo.WindowWidth,
                                   AppInfo.WindowHeight);
        s_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
    }
    if (!s_ImageView) {
        s_ImageView = CreateS<ImageView>(AppInfo.WindowWidth, AppInfo.WindowHeight,
                                         AppInfo.ImageWidth, AppInfo.ImageHeight);
    }
    m_DeviceManager = CreateU<RenderDeviceManager>();
    UI::Init();
    RT_LOG("Application Initialised");
}

Application::~Application() {}

void Application::Run() {
    RT_PROFILE_FUNC;

    while (!s_Window->ShouldClose()) {
        m_DeviceManager->OnUpdate();
        s_ImageView->OnUpdate();
        UI::PreRender();
        UI();
        if (UI::HasChanged()) {
            m_DeviceManager->Dirty();
        }
        UI::Render();
        s_Window->OnUpdate();
    }
}

void Application::UI() {
    RT_PROFILE_FUNC;

    ImGui::Begin("Controls");
    ImGui::BeginDisabled(s_DisableUI);
    m_DeviceManager->UI();
    s_ImageView->UI();
    ImGui::EndDisabled();
    ImGui::End();
}

void Application::OnEvent(Event& event) {
    RT_PROFILE_FUNC;

    if (UI::WantInput()) {
        return;
    }

    s_ImageView->OnEvent(event);
    m_DeviceManager->OnEvent(event);

    if (event.IsDirty()) {
        m_DeviceManager->Dirty();
    }
}

}  // namespace raytracing