#include "UI.h"

#include "App/Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace raytracing {

bool UI::s_DirtyFlag = false;

void UI::Init() {
    RT_PROFILE_FUNC;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow()->GetGLFWWindow(),
                                 true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void UI::PreRender() {
    RT_PROFILE_FUNC;
    /* ImGui Frame */
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::Render() {
    RT_PROFILE_FUNC;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    s_DirtyFlag = false;
}

bool UI::WantInput() {
    RT_PROFILE_FUNC;
    ImGuiIO& io = ImGui::GetIO();
    return (io.WantCaptureMouse || io.WantCaptureKeyboard);
}

}  // namespace raytracing