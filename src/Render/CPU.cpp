#include "CPU.h"

#include "Application.h"
#include "Kernel/CPU/CircleTest.h"
#include "Kernel/CPU/ColorTest.h"
#include "Kernel/CPU/Learn.h"
#include "imgui.h"

namespace raytracing {

CPUDevice::CPUDevice() : RenderDevice("CPU") {
    m_Kernels.AddKernel(new ColorTestKernel());
    m_Kernels.AddKernel(new CircleTestKernel());
    m_Kernels.AddKernel(new LearnKernel());

    m_ThreadPool = new ThreadPool(m_NumTilesX * m_NumTilesY);
}

CPUDevice::~CPUDevice() {
    delete m_ThreadPool;
}

void CPUDevice::OnUpdate(Image* image) {
    RT_PROFILE_FUNC;

    if (m_RealTimeExecution) {
        Execute(image);
    }
}

void CPUDevice::Execute(Image* image) {
    RT_PROFILE_FUNC;

    // TODO: Better interface? Then user can use quick_stop flag without function param if they want
    // m_ThreadPool->AddTaskQE([image, x, y, s, ] (std::atomic<bool>& quick_stop) -> void {
    //     image->PerSampleSection();
    // });

    if (!m_ThreadPool->IsRunning()) {
        // TODO: Reimplement timing

        if (m_Multithreaded) {
            ExecuteThreaded(image);
        } else {
            m_ThreadPool->Resize(1);
            m_ThreadPool->AddTaskQE(
                &Image::PerSample, image,
                [this](Image* image, uint32_t x, uint32_t y, uint32_t s) {
                    return m_Kernels.GetCurrentKernel()->Exec(image, x, y, s);
                },
                m_NumSamples);
        }
    }
}

/* TODO: Handle odd image dimensions / tiles. */
void CPUDevice::ExecuteThreaded(Image* image) {
    RT_PROFILE_FUNC;

    m_ThreadPool->Resize(m_NumTilesX * m_NumTilesY);
    uint32_t tile_width = image->GetWidth() / m_NumTilesX;
    uint32_t tile_height = image->GetHeight() / m_NumTilesY;
    for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
        for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
            uint32_t x = tx * tile_width;
            uint32_t y = ty * tile_height;

            m_ThreadPool->AddTaskQE(
                &Image::PerSampleSection, image,
                [this](Image* image, uint32_t x, uint32_t y, uint32_t s) {
                    return m_Kernels.GetCurrentKernel()->Exec(image, x, y, s);
                },
                m_NumSamples, x, y, tile_width, tile_height);
        }
    }
}

void CPUDevice::SettingsUI() {
    ImGui::Checkbox("Multithreaded", &m_Multithreaded);
    ImGui::SliderInt("X Tiles", (int*)&m_NumTilesX, 1, 8);
    ImGui::SliderInt("Y Tiles", (int*)&m_NumTilesY, 1, 8);
    ImGui::SliderInt("Samples", (int*)&m_NumSamples, 1, 100);

    ImGui::Separator();

    ImGui::Text("CPU Execution");

    // Execute and clear buttons are only enabled when real time execution is turned off and threadpool is not running
    bool disabled = false;
    if (m_RealTimeExecution || m_ThreadPool->IsRunning()) {
        ImGui::BeginDisabled();
        disabled = true;
    }

    if (ImGui::Button("Execute")) {
        Execute(Application::GetImageView()->GetImage());
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        Application::GetImageView()->GetImage()->Randomize();
    }

    if (disabled) {
        ImGui::EndDisabled();
    }
    //

    if (m_RealTimeExecution || !m_ThreadPool->IsRunning()) {
        if (ExecutionTime != 0.0f) {
            ImGui::SameLine();
            ImGui::Text("%.3fs", ExecutionTime);
        }
    }

    if (!m_RealTimeExecution && m_ThreadPool->IsRunning()) {
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            m_ThreadPool->Clear();
        }
    }

    ImGui::Checkbox("Real Time", &m_RealTimeExecution);
}

}  // namespace raytracing