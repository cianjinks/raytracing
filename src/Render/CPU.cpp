#include "CPU.h"

#include "Kernel/CPU/CircleTest.h"
#include "Kernel/CPU/Learn.h"
#include "imgui.h"

namespace raytracing {

CPUDevice::CPUDevice() : RenderDevice("CPU") {
    m_Kernels.AddKernel(new CircleTestKernel());
    m_Kernels.AddKernel(new LearnKernel());
}

CPUDevice::~CPUDevice() {}

void CPUDevice::Execute(Image* image) {
    RT_LOG("CPU Execute!");
    image->PerPixel([this](Image* image, uint32_t x, uint32_t y) {
        return m_Kernels.GetCurrentKernel()->Exec(image, x, y);
    });
}

void CPUDevice::SettingsUI() { ImGui::Text("CPU device settings go here."); }

}  // namespace raytracing