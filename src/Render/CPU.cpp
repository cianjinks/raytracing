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

void CPUDevice::Execute(Image* image) {
    if (m_Multithreaded) {
        std::thread t(&CPUDevice::ExecuteThreaded, this, image);
        t.detach();
    } else {
        UpdateImage(image);
    }
}

/* TODO: Divide image into sections based on thread count. */
void CPUDevice::ExecuteThreaded(Image* image) { UpdateImage(image); }

void CPUDevice::UpdateImage(Image* image) {
    Executing = true;
    image->PerSample(
        [this](Image* image, uint32_t x, uint32_t y, uint32_t s) {
            return m_Kernels.GetCurrentKernel()->Exec(image, x, y);
        },
        m_NumSamples);
    Executing = false;
}

void CPUDevice::SettingsUI() {
    ImGui::Checkbox("Multithreaded", &m_Multithreaded);
    ImGui::SliderInt("Samples", (int*)&m_NumSamples, 1, 100);
}

}  // namespace raytracing