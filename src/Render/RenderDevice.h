#pragma once

#include "Window/Image.h"

namespace raytracing {

class RenderDevice {
   public:
    std::string Name;

   public:
    RenderDevice(std::string name);
    ~RenderDevice();

    virtual void Execute(Image* image) = 0;
    virtual void SettingsUI() = 0;
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
};

}  // namespace raytracing