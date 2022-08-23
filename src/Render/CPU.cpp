#include "CPU.h"

#include "Kernel/CPU/CircleTest.h"
#include "Kernel/CPU/ColorTest.h"
#include "Kernel/CPU/Learn.h"
#include "imgui.h"

namespace raytracing {

CPUDevice::CPUDevice() : RenderDevice("CPU") {
    m_Kernels.AddKernel(new ColorTestKernel());
    m_Kernels.AddKernel(new CircleTestKernel());
    m_Kernels.AddKernel(new LearnKernel());
}

CPUDevice::~CPUDevice() {}

float CPUDevice::Execute(Image* image) {
    Timer::Start();
    image->PerPixel([this](Image* image, uint32_t x, uint32_t y) {
        return m_Kernels.GetCurrentKernel()->Exec(image, x, y);
    });
    Timer::End();
    return Timer::GetElapsedTimeMS();
}

void CPUDevice::SettingsUI() { ImGui::Text("CPU device settings go here."); }

}  // namespace raytracing