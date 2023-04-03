#pragma once

#include "RenderDevice.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    uint32_t m_NumSamples = 25;
    bool m_Multithreaded = true;
    uint32_t m_NumTilesX = 8;
    uint32_t m_NumTilesY = 8;

   public:
    CPUDevice();
    ~CPUDevice();

    void Execute(Image* image) override;
    void SettingsUI() override;

    void ExecuteThreaded(Image* image);
    void UpdateImage(Image* image);
};

}  // namespace raytracing