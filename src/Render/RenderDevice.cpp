#include "RenderDevice.h"

#include "Application.h"
#include "CPU.h"
#include "imgui.h"

namespace raytracing {

RenderDevice::RenderDevice(std::string name) : Name(name) {}

RenderDeviceManager::RenderDeviceManager() { SetupRenderDevices(); }

RenderDeviceManager::~RenderDeviceManager() { ClearRenderDevices(); }

void RenderDeviceManager::SetupRenderDevices() {
    /* Add more here. */
    RenderDevice* cpu = new CPUDevice();
    m_DeviceList.emplace_back(cpu);
    m_CurrentDevice = cpu;
    m_CurrentDeviceIndex = 0;
}

void RenderDeviceManager::ClearRenderDevices() {
    for (RenderDevice* rd : m_DeviceList) {
        delete rd;
    }
    m_DeviceList.clear();
    m_CurrentDevice = nullptr;
    m_CurrentDeviceIndex = 0;
}

void RenderDeviceManager::UI() {
    ImGui::Text("Devices");
    DeviceComboUI();
    m_CurrentDevice->GetKernels().DeviceComboUI();
    ImGui::Separator();

    ImGui::Text("Device Settings");
    m_CurrentDevice->SettingsUI();
    ImGui::Separator();

    ImGui::Text("Kernel Settings");
    m_CurrentDevice->GetCurrentKernel()->UI();
    ImGui::Separator();

    ImGui::Text("Execution");
    ImGui::Checkbox("Live", &m_LiveExecToggle);
    if (m_LiveExecToggle) {
        m_CurrentDevice->Execute(Application::GetImageView()->GetImage());
    } else {
        ImGui::SameLine();
        if (ImGui::Button("Execute")) {
            m_CurrentDevice->Execute(Application::GetImageView()->GetImage());
        }
    }
}

void RenderDeviceManager::DeviceComboUI() {
    if (ImGui::BeginCombo("Device", m_CurrentDevice->Name.c_str())) {
        for (int r = 0; r < m_DeviceList.size(); r++) {
            RenderDevice* rd = m_DeviceList[r];
            const bool is_selected = (r == m_CurrentDeviceIndex);
            if (ImGui::Selectable(rd->Name.c_str(), is_selected)) {
                m_CurrentDevice = rd;
                m_CurrentDeviceIndex = r;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

}  // namespace raytracing