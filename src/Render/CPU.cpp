#include "CPU.h"

#include "Application.h"
#include "Kernel/CPU/CircleTest.h"
#include "Kernel/CPU/ColorTest.h"
#include "Kernel/CPU/Learn.h"
#include "imgui.h"

namespace raytracing {

CPUDevice::CPUDevice(Texture2D<uint8_t, 3>* texture)
    : RenderDevice("CPU"),
      m_Texture(texture),
      m_AccumulationBuffer(texture->GetWidth(), texture->GetHeight()) {
    m_Kernels.AddKernel(new ColorTestKernel());
    m_Kernels.AddKernel(new CircleTestKernel());
    m_Kernels.AddKernel(new LearnKernel());

    m_ThreadPool = new ThreadPool(m_NumTilesX * m_NumTilesY);
}

CPUDevice::~CPUDevice() {
    delete m_ThreadPool;
}

void CPUDevice::OnUpdate() {
    RT_PROFILE_FUNC;

    if (m_RealTimeExecution) {
        Execute();
    }
}

void CPUDevice::Execute() {
    RT_PROFILE_FUNC;

    // TODO: Resizing accumulation buffer?

    if (!m_ThreadPool->IsActive()) {
        // TODO: Reimplement timing

        if (m_Multithreaded) {
            ExecuteThreaded();
        } else {
            ExecuteSingle();
        }
    }
}

/* TODO: Handle odd image dimensions / tiles. */
void CPUDevice::ExecuteThreaded() {
    RT_PROFILE_FUNC;

    m_ThreadPool->Resize(m_NumTilesX * m_NumTilesY);
    m_AccumulationBuffer.Clear();

    uint32_t tile_width = m_Texture->GetWidth() / m_NumTilesX;
    uint32_t tile_height = m_Texture->GetHeight() / m_NumTilesY;
    for (uint32_t tx = 0; tx < m_NumTilesX; tx++) {
        for (uint32_t ty = 0; ty < m_NumTilesY; ty++) {
            uint32_t x = tx * tile_width;
            uint32_t y = ty * tile_height;

            auto func = [this, x, y, tile_width, tile_height](std::atomic<bool>& stop) -> void {
                for (uint32_t s = 0; s < m_NumSamples; s++) {
                    for (uint32_t w = x; w < (x + tile_width); w++) {
                        for (uint32_t h = y; h < (y + tile_height); h++) {
                            m_AccumulationBuffer(w, h) += m_Kernels.GetCurrentKernel()->Exec(m_Texture, w, h, s);
                            glm::vec3 val = glm::sqrt(m_AccumulationBuffer(w, h) / float(s + 1));
                            m_Texture->at(w, h) = glm::u8vec3(glm::clamp(val, 0.0f, 0.999f) * 256.0f);
                            if (stop) { return; }
                        }
                    }
                }
            };
            m_ThreadPool->AddTaskQE(func);
        }
    }
}

void CPUDevice::ExecuteSingle() {
    RT_PROFILE_FUNC;

    m_ThreadPool->Resize(1);
    m_AccumulationBuffer.Clear();

    auto func = [this](std::atomic<bool>& stop) -> void {
        for (uint32_t s = 0; s < m_NumSamples; s++) {
            for (uint32_t w = 0; w < m_Texture->GetWidth(); w++) {
                for (uint32_t h = 0; h < m_Texture->GetHeight(); h++)
                {
                    m_AccumulationBuffer(w, h) += m_Kernels.GetCurrentKernel()->Exec(m_Texture, w, h, s);
                    glm::vec3 val = glm::sqrt(m_AccumulationBuffer(w, h) / float(s + 1));
                    m_Texture->at(w, h) = glm::u8vec3(glm::clamp(val, 0.0f, 0.999f) * 256.0f);
                    if (stop) { return; }
                }
            }
        }
    };
    m_ThreadPool->AddTaskQE(func);
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