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

    // CPU writes to ImageView's cpu texture buffer
    m_Texture = Application::GetImageView()->GetTexture();
    m_AccumulationBuffer = new Texture2D3f(m_Texture->GetWidth(), m_Texture->GetHeight());

    m_ThreadPool = new ThreadPool(m_NumTilesX * m_NumTilesY);
}

CPUDevice::~CPUDevice() {
    delete m_ThreadPool;
}

void CPUDevice::OnUpdate() {
    RT_PROFILE_FUNC;

    if (m_RealTimeExecution) {
        // TODO: Create a dirty/reset function
        if (m_CurrentSample == 0) {
            m_AccumulationBuffer->Resize(m_Texture->GetWidth(), m_Texture->GetHeight());
        }
        if (m_CurrentSample < m_NumSamples) {
            if (m_Multithreaded) {
                ExecuteThreadedRT();
            } else {
                ExecuteThreaded();
            }
            m_CurrentSample++;
        }
    }
}

void CPUDevice::Execute() {
    RT_PROFILE_FUNC;
    if (!m_ThreadPool->IsActive()) {
        // TODO: Reimplement timing

        m_AccumulationBuffer->Resize(m_Texture->GetWidth(), m_Texture->GetHeight());
        if (m_Multithreaded) {
            ExecuteThreaded();
        } else {
            ExecuteSingle();
        }
    }
}

void CPUDevice::ExecuteThreaded() {
    RT_PROFILE_FUNC;
    /* TODO: Handle odd image dimensions / tiles. */
    m_ThreadPool->Resize(m_NumTilesX * m_NumTilesY);
    uint32_t tile_width = m_Texture->GetWidth() / m_NumTilesX;
    uint32_t tile_height = m_Texture->GetHeight() / m_NumTilesY;
    for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
        for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
            uint32_t x = tx * tile_width;
            uint32_t y = ty * tile_height;
            m_ThreadPool->AddTaskQE([this, x, y, tile_width, tile_height](std::atomic<bool>& stop) -> void {
                for (uint32_t s = 0; s < m_NumSamples; s++) {
                    this->AccumulateSection(x, y, s, tile_width, tile_height, stop);
                }
            });
        }
    }
}

void CPUDevice::ExecuteSingle() {
    RT_PROFILE_FUNC;
    m_ThreadPool->Resize(1);
    m_ThreadPool->AddTaskQE([this](std::atomic<bool>& stop) -> void {
        for (uint32_t s = 0; s < m_NumSamples; s++) {
            this->AccumulateSection(0, 0, s, m_Texture->GetWidth(), m_Texture->GetHeight(), stop);
        }
    });
}

void CPUDevice::ExecuteThreadedRT() {
    RT_PROFILE_FUNC;
    /* TODO: Handle odd image dimensions / tiles. */
    m_ThreadPool->Resize(m_NumTilesX * m_NumTilesY);
    uint32_t tile_width = m_Texture->GetWidth() / m_NumTilesX;
    uint32_t tile_height = m_Texture->GetHeight() / m_NumTilesY;
    for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
        for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
            uint32_t x = tx * tile_width;
            uint32_t y = ty * tile_height;
            m_ThreadPool->AddTaskQE(&CPUDevice::AccumulateSection, this, m_CurrentSample, x, y, tile_width, tile_height);
        }
    }
    m_ThreadPool->WaitForTasks();
}

void CPUDevice::ExecuteSingleRT() {
    RT_PROFILE_FUNC;

    std::atomic_bool stop = false;
    AccumulateSection(0, 0, m_CurrentSample, m_Texture->GetWidth(), m_Texture->GetHeight(), stop);
}

void CPUDevice::AccumulateSection(uint32_t x, uint32_t y, uint32_t s, uint32_t width, uint32_t height, std::atomic<bool>& stop) {
    RT_PROFILE_FUNC;

    for (uint32_t w = x; w < (x + width); w++) {
        for (uint32_t h = y; h < (y + height); h++) {
            m_AccumulationBuffer->at(w, h) += m_Kernels.GetCurrentKernel()->Exec(m_Texture, w, h, s);
            glm::vec3 val = glm::sqrt(m_AccumulationBuffer->at(w, h) / float(s + 1));
            m_Texture->at(w, h) = glm::u8vec3(glm::clamp(val, 0.0f, 0.999f) * 256.0f);
            if (stop) { return; }
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
    if (m_RealTimeExecution || m_ThreadPool->IsActive()) {
        ImGui::BeginDisabled();
        disabled = true;
    }

    if (ImGui::Button("Execute")) {
        Execute();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        m_Texture->Randomize();
    }

    if (disabled) {
        ImGui::EndDisabled();
    }
    //

    if (m_RealTimeExecution || !m_ThreadPool->IsActive()) {
        if (ExecutionTime != 0.0f) {
            ImGui::SameLine();
            ImGui::Text("%.3fs", ExecutionTime);
        }
    }

    if (!m_RealTimeExecution && m_ThreadPool->IsActive()) {
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            m_ThreadPool->Clear();
        }
    }

    ImGui::Checkbox("Real Time", &m_RealTimeExecution);
}

}  // namespace raytracing