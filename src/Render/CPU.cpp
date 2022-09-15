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
    image->PerSample(
        [this](Image* image, uint32_t x, uint32_t y, uint32_t s) {
            return m_Kernels.GetCurrentKernel()->Exec(image, x, y);
        },
        m_NumSamples);
    Timer::End();
    return Timer::GetElapsedTimeMS();
}

void CPUDevice::SettingsUI() {
    ImGui::SliderInt("Samples", (int*)&m_NumSamples, 1, 100);
}

}  // namespace raytracing