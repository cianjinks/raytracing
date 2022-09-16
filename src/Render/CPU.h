#pragma once

#include "RenderDevice.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    uint32_t m_NumSamples = 1;
    bool m_Multithreaded = true;

   public:
    CPUDevice();
    ~CPUDevice();

    void Execute(Image* image) override;
    void SettingsUI() override;

    void ExecuteThreaded(Image* image);
    void UpdateImage(Image* image);
};

}  // namespace raytracing