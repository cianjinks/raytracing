#pragma once

#include "RenderDevice.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    uint32_t m_NumSamples = 20;

   public:
    CPUDevice();
    ~CPUDevice();

    float Execute(Image* image) override;
    void SettingsUI() override;
};

}  // namespace raytracing