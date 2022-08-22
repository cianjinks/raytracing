#include "Kernel.h"

#include "imgui.h"

namespace raytracing {

Kernel::Kernel(std::string name) : Name(name) {}

KernelLibrary::KernelLibrary() {}

KernelLibrary::~KernelLibrary() {
    for (Kernel* k : m_KernelList) {
        delete k;
    }
    m_KernelList.clear();
    m_CurrentKernel = nullptr;
    m_CurrentKernelIndex = 0;
}

void KernelLibrary::AddKernel(Kernel* kernel) {
    m_KernelList.emplace_back(kernel);
    m_CurrentKernel = kernel;
    m_CurrentKernelIndex = m_KernelList.size() - 1;
}

void KernelLibrary::SetCurrentKernel(uint32_t index) {
    if (index < m_KernelList.size()) {
        m_CurrentKernel = m_KernelList[index];
        m_CurrentKernelIndex = index;
    } else {
        RT_ERROR("Kernel List Index OOB!");
    }
}

void KernelLibrary::DeviceComboUI() {
    if (ImGui::BeginCombo("Kernel", m_CurrentKernel->Name.c_str())) {
        for (int k = 0; k < m_KernelList.size(); k++) {
            Kernel* kernel = m_KernelList[k];
            const bool is_selected = (k == m_CurrentKernelIndex);
            if (ImGui::Selectable(kernel->Name.c_str(), is_selected)) {
                m_CurrentKernel = kernel;
                m_CurrentKernelIndex = k;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

}  // namespace raytracing