#include "Kernel.h"

#include "Window/UI.h"

namespace raytracing {

Kernel::Kernel(std::string name) : Name(name) {}

KernelLibrary::KernelLibrary() {}

KernelLibrary::~KernelLibrary() {
    m_KernelList.clear();
    m_CurrentKernel = nullptr;
    m_CurrentKernelIndex = 0;
}

void KernelLibrary::AddKernel(S<Kernel> kernel) {
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
            S<Kernel> kernel = m_KernelList[k];
            const bool is_selected = (k == m_CurrentKernelIndex);
            if (UI::Selectable(kernel->Name.c_str(), is_selected)) {
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