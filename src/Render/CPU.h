#pragma once

#include "RenderDevice.h"
#include "Util/Thread.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    uint32_t m_NumSamples = 1;
    bool m_Multithreaded = true;
    uint32_t m_NumTilesX = 4;
    uint32_t m_NumTilesY = 4;

    bool m_RealTimeExecution = false;
    ThreadPool* m_ThreadPool;

   public:
    CPUDevice();
    ~CPUDevice();

    void OnUpdate(Image* image) override;
    void SettingsUI() override;

   private:
    void Execute(Image* image);
    void ExecuteThreaded(Image* image);
};

}  // namespace raytracing