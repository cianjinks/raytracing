#pragma once

#include "RenderDevice.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   public:
    CPUDevice();
    ~CPUDevice();

    float Execute(Image* image) override;
    void SettingsUI() override;
};

}  // namespace raytracing