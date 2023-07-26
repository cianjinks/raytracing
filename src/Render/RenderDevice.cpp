#include "RenderDevice.h"

#include "Application.h"
#include "CPU.h"
#include "Window/UI.h"

namespace raytracing {

RenderDevice::RenderDevice(std::string name) : Name(name) {}

RenderDeviceManager::RenderDeviceManager() { SetupRenderDevices(); }

RenderDeviceManager::~RenderDeviceManager() { ClearRenderDevices(); }

void RenderDeviceManager::SetupRenderDevices() {
    RT_PROFILE_FUNC;

    /* Add more here. */
    RenderDevice* cpu = new CPUDevice();  // TODO: CPU threadpool should be freed when device is swapped
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

void RenderDeviceManager::OnUpdate() {
    if (m_CurrentDevice->GetCurrentKernel()->OnUpdate() || m_IsDirty) {
        m_CurrentDevice->Dirty();
        m_IsDirty = false;
    }
    m_CurrentDevice->OnUpdate();
}

void RenderDeviceManager::OnEvent(Event& event) {
    m_CurrentDevice->GetCurrentKernel()->OnEvent(event);
    m_CurrentDevice->OnEvent(event);
}

void RenderDeviceManager::UI() {
    RT_PROFILE_FUNC;

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
}

void RenderDeviceManager::DeviceComboUI() {
    if (ImGui::BeginCombo("Device", m_CurrentDevice->Name.c_str())) {
        for (int r = 0; r < m_DeviceList.size(); r++) {
            RenderDevice* rd = m_DeviceList[r];
            const bool is_selected = (r == m_CurrentDeviceIndex);
            if (UI::Selectable(rd->Name.c_str(), is_selected)) {
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