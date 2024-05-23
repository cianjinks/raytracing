#include "CPU.h"

#include "App/Application.h"
#include "Kernel/CPU/CircleTest.h"
#include "Kernel/CPU/ColorTest.h"
#include "Kernel/CPU/Learn.h"
#include "Window/UI.h"

namespace raytracing {

CPUDevice::CPUDevice() : RenderDevice("CPU") {
    m_Kernels.AddKernel(CreateS<ColorTestKernel>());
    m_Kernels.AddKernel(CreateS<CircleTestKernel>());
    m_Kernels.AddKernel(CreateS<LearnKernel>());

    // CPU writes to ImageView's cpu texture buffer
    m_Texture = Application::GetImageView()->GetTexture();
    m_AccumulationBuffer = CreateU<Texture2D3f>(m_Texture->GetWidth(), m_Texture->GetHeight());

    m_ThreadPool = CreateU<ThreadPool>(m_NumThreads);
}

CPUDevice::~CPUDevice() {}

void CPUDevice::OnUpdate() {
    RT_PROFILE_FUNC;

    /* Catch end of once-off execution. */
    if (m_IsExecuting && !m_ThreadPool->IsActive()) {
        m_IsExecuting = false;
        Application::EnableUI();

        if (m_Timer.IsRunning()) {
            m_Timer.Stop();
            m_ExecutionTime = m_Timer.GetElapsedTimeS();
        }
    }

    /* Real time execution */
    if (m_RealTimeExecution) {
        if (m_CurrentSample < m_NumSamples) {
            m_Timer.Start();

            if (m_Multithreaded) {
                ExecuteThreadedRT();
            } else {
                ExecuteSingleRT();
            }
            m_CurrentSample++;

            m_Timer.Stop();
            m_ExecutionTime = m_Timer.GetElapsedTimeS();
        }
    }
}

void CPUDevice::Dirty() {
    if (m_RealTimeExecution) {
        m_Texture->Clear();
        m_AccumulationBuffer->Resize(m_Texture->GetWidth(), m_Texture->GetHeight());
        m_CurrentSample = 0;
    }
}

bool CPUDevice::Execute() {
    RT_PROFILE_FUNC;
    if (!m_ThreadPool->IsActive() && !m_IsExecuting) {
        m_Timer.Start();

        m_Texture->Clear();
        m_AccumulationBuffer->Resize(m_Texture->GetWidth(), m_Texture->GetHeight());
        if (m_Multithreaded) {
            ExecuteThreaded();
        } else {
            ExecuteSingle();
        }

        /* Flag so we can catch end of once-off execution in OnUpdate. */
        m_IsExecuting = true;
    }
    return m_IsExecuting;
}

void CPUDevice::ExecuteThreaded() {
    RT_PROFILE_FUNC;
    /* TODO: Handle odd image dimensions / tiles. */
    m_ThreadPool->Resize(m_NumThreads);
    uint32_t tile_width = m_Texture->GetWidth() / m_NumTilesX;
    uint32_t tile_height = m_Texture->GetHeight() / m_NumTilesY;
    for (uint32_t s = 0; s < m_NumSamples; s++) {
        for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
            for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
                uint32_t x = tx * tile_width;
                uint32_t y = ty * tile_height;
                m_ThreadPool->AddTaskQE(&CPUDevice::AccumulateSection, this, x, y, s, tile_width, tile_height);
            }
        }
        // m_CurrentSample++ /* Not possible with this setup. */
        /* We don't wait for the threadpool to finish each sample before queueing the next. Could do that if we move once-off execution to the OnUpdate function. */
    }
}

void CPUDevice::ExecuteSingle() {
    RT_PROFILE_FUNC;
    m_ThreadPool->Resize(1);
    m_ThreadPool->AddTaskQE([this](std::atomic<bool>& stop) -> void {
        for (uint32_t s = 0; s < m_NumSamples; s++) {
            this->AccumulateSection(0, 0, s, m_Texture->GetWidth(), m_Texture->GetHeight(), stop);
            m_CurrentSample++;
        }
    });
}

void CPUDevice::ExecuteThreadedRT() {
    RT_PROFILE_FUNC;
    /* TODO: Handle odd image dimensions / tiles. */
    m_ThreadPool->Resize(m_NumThreads);
    uint32_t tile_width = m_Texture->GetWidth() / m_NumTilesX;
    uint32_t tile_height = m_Texture->GetHeight() / m_NumTilesY;
    for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
        for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
            uint32_t x = tx * tile_width;
            uint32_t y = ty * tile_height;
            m_ThreadPool->AddTaskQE(&CPUDevice::AccumulateSection, this, x, y, m_CurrentSample, tile_width, tile_height);
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
    // Note: This loop order provides optimal cache access of the texture
    for (uint32_t h = y; h < (y + height); h++) {
        for (uint32_t w = x; w < (x + width); w++) {
            uint32_t seed = (s * m_Texture->GetWidth() * m_Texture->GetHeight()) + (h * m_Texture->GetWidth()) + w;
            m_AccumulationBuffer->at(w, h) += m_Kernels.GetCurrentKernel()->Exec(m_Texture, w, h, s, seed);
            glm::vec3 val = glm::sqrt(m_AccumulationBuffer->at(w, h) / float(s + 1));
            m_Texture->at(w, h) = glm::u8vec3(glm::clamp(val, 0.0f, 0.999f) * 256.0f);
            if (stop) {
                return;
            }
        }
    }
}

void CPUDevice::SettingsUI() {
    UI::Checkbox("Multithreaded", &m_Multithreaded);
    UI::SliderInt("Threads", (int*)&m_NumThreads, 1, ThreadPool::s_MaxThreadCount);
    UI::SliderInt("X Tiles", (int*)&m_NumTilesX, 1, 8);
    UI::SliderInt("Y Tiles", (int*)&m_NumTilesY, 1, 8);
    UI::SliderInt("Samples", (int*)&m_NumSamples, 1, 500);

    ImGui::Separator();

    ImGui::Text("CPU Execution");

    // Execute and clear buttons are only enabled when real time execution is turned off and threadpool is not running
    bool disabled = false;
    if (m_RealTimeExecution || m_ThreadPool->IsActive()) {
        ImGui::BeginDisabled();
        disabled = true;
    }

    if (UI::Button("Execute")) {
        if (Execute()) {
            Application::DisableUI();
        }
    }

    ImGui::SameLine();
    if (UI::Button("Clear")) {
        m_Texture->Randomize();
    }

    if (disabled) {
        ImGui::EndDisabled();
    }
    //

    // TODO: Cleanup
    if (!m_RealTimeExecution && m_ThreadPool->IsActive()) {
        ImGui::SameLine();
        if (Application::IsUIDisabled()) {
            ImGui::EndDisabled();
        }

        if (UI::Button("Stop")) {
            m_ThreadPool->Clear();
        }

        if (Application::IsUIDisabled()) {
            ImGui::BeginDisabled();
        }
    }

    if (m_RealTimeExecution || !m_ThreadPool->IsActive()) {
        if (m_ExecutionTime != 0.0f) {
            ImGui::SameLine();
            ImGui::Text("%.3fs", m_ExecutionTime);
        }
        ImGui::SameLine();
        ImGui::Text("(%u/%u samples)", m_CurrentSample, m_NumSamples);
    }

    UI::Checkbox("Real Time", &m_RealTimeExecution);
}

}  // namespace raytracing