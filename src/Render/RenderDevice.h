#pragma once

#include "Kernel/Kernel.h"
#include "Window/Image.h"

namespace raytracing {

class RenderDevice {
   public:
    std::string Name;

   protected:
    KernelLibrary m_Kernels;

   public:
    RenderDevice(std::string name);
    virtual ~RenderDevice() = default;

    virtual void Execute(Image* image) = 0;
    virtual void SettingsUI() = 0;

    KernelLibrary& GetKernels() { return m_Kernels; }
    Kernel* GetCurrentKernel() { return m_Kernels.GetCurrentKernel(); }
};

class RenderDeviceManager {
   private:
    std::vector<RenderDevice*> m_DeviceList;
    RenderDevice* m_CurrentDevice = nullptr;
    uint32_t m_CurrentDeviceIndex = 0;

   public:
    RenderDeviceManager();
    ~RenderDeviceManager();

    /* UI for device selection, settings, execution options. */
    void UI();

    RenderDevice* GetCurrentDevice() const { return m_CurrentDevice; }

   private:
    void SetupRenderDevices();
    void ClearRenderDevices();

    void DeviceComboUI();
};

}  // namespace raytracing