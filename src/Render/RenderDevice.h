#pragma once

#include "Kernel/Kernel.h"

namespace raytracing {

class RenderDevice {
   public:
    std::string Name;

   protected:
    KernelLibrary m_Kernels;

    float m_ExecutionTime = 0.0f;
    bool m_Dirty = false;

   public:
    RenderDevice(std::string name);
    virtual ~RenderDevice() = default;

    virtual void OnUpdate() = 0;
    virtual void OnEvent(Event& event) = 0;
    virtual void SettingsUI() = 0;

    /* Called when any UI setting changes in the application or kernel updates (Kernel::OnUpdate). */
    virtual void Dirty() = 0;

    KernelLibrary& GetKernels() { return m_Kernels; }
    Kernel* GetCurrentKernel() { return m_Kernels.GetCurrentKernel(); }
};

class RenderDeviceManager {
   private:
    std::vector<RenderDevice*> m_DeviceList;
    RenderDevice* m_CurrentDevice = nullptr;
    uint32_t m_CurrentDeviceIndex = 0;

    bool m_IsDirty = false;

   public:
    RenderDeviceManager();
    ~RenderDeviceManager();

    void OnUpdate();
    void OnEvent(Event& event);
    void UI();

    void Dirty() { m_IsDirty = true; }

    RenderDevice* GetCurrentDevice() const { return m_CurrentDevice; }

   private:
    void SetupRenderDevices();
    void ClearRenderDevices();

    void DeviceComboUI();
};

}  // namespace raytracing