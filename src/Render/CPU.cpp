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
    RT_PROFILE_FUNC;

    if (m_Multithreaded) {
        std::thread t(&CPUDevice::ExecuteThreaded, this, image);
        t.detach();
    } else {
        UpdateImage(image);
    }
}

/* TODO: Handle odd image dimensions / tiles. */
void CPUDevice::ExecuteThreaded(Image* image) {
    RT_PROFILE_FUNC;

    Executing = true;
    Timer::Start();

    std::vector<std::thread> threads(m_NumTilesX * m_NumTilesY);
    uint32_t tile_width = image->GetWidth() / m_NumTilesX;
    uint32_t tile_height = image->GetHeight() / m_NumTilesY;
    for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
        for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
            uint32_t x = tx * tile_width;
            uint32_t y = ty * tile_height;
            threads.push_back(std::thread(
                &Image::PerSampleSection, image,
                [this](Image* image, uint32_t x, uint32_t y, uint32_t s) {
                    return m_Kernels.GetCurrentKernel()->Exec(image, x, y);
                },
                m_NumSamples, x, y, tile_width, tile_height));
        }
    }

    for (uint32_t t = 0; t < threads.size(); t++) {
        if (threads[t].joinable()) {
            threads[t].join();
        }
    }

    Timer::End();
    ExecutionTime = Timer::GetElapsedTimeS();
    Executing = false;
}

void CPUDevice::UpdateImage(Image* image) {
    RT_PROFILE_FUNC;

    Executing = true;
    Timer::Start();

    image->PerSample(
        [this](Image* image, uint32_t x, uint32_t y, uint32_t s) {
            return m_Kernels.GetCurrentKernel()->Exec(image, x, y);
        },
        m_NumSamples);

    Timer::End();
    ExecutionTime = Timer::GetElapsedTimeS();
    Executing = false;
}

void CPUDevice::SettingsUI() {
    ImGui::Checkbox("Multithreaded", &m_Multithreaded);
    ImGui::SliderInt("X Tiles", (int*)&m_NumTilesX, 1, 8);
    ImGui::SliderInt("Y Tiles", (int*)&m_NumTilesY, 1, 8);
    ImGui::SliderInt("Samples", (int*)&m_NumSamples, 1, 100);
}

}  // namespace raytracing