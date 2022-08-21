#pragma once

#include "RenderDevice.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    int m_CircleRadius = 25.0f;

   public:
    CPUDevice();
    ~CPUDevice();

    void Execute(Image* image) override;
    void SettingsUI() override;
};

}  // namespace raytracing