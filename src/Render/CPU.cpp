#include "CPU.h"

#include "imgui.h"

namespace raytracing {

CPUDevice::CPUDevice() : RenderDevice("CPU") {}

void CPUDevice::Execute(Image* image) {
    RT_LOG("CPU Execute!");
    image->Randomize();
}

void CPUDevice::SettingsUI() {
    ImGui::Text("This is a test!");
    if (ImGui::Button("Button")) {
    }
}

}  // namespace raytracing