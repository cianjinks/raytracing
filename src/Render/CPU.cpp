#include "CPU.h"

#include "imgui.h"

namespace raytracing {

CPUDevice::CPUDevice() : RenderDevice("CPU") {}

CPUDevice::~CPUDevice() {}

void CPUDevice::Execute(Image* image) {
    RT_LOG("CPU Execute!");
    image->PerPixel([this, image](uint32_t x, uint32_t y) {
        x -= image->GetWidth() / 2;
        y -= image->GetHeight() / 2;
        if ((x * x) + (y * y) <= (uint32_t)m_CircleRadius) {
            return Pixel{0, 255, 0};
        }
        return Pixel{255, 0, 0};
    });
}

void CPUDevice::SettingsUI() {
    ImGui::SliderInt("Circle Radius", &m_CircleRadius, 0, 1000);
}

}  // namespace raytracing