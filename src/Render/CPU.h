#pragma once

#include "RenderDevice.h"
#include "Texture2D.h"
#include "Util/Thread.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    uint32_t m_NumSamples = 1;
    bool m_Multithreaded = true;
    uint32_t m_NumTilesX = 4;
    uint32_t m_NumTilesY = 4;
    Texture2D<float, 3> m_AccumulationBuffer;
    Texture2D<uint8_t, 3>* m_Texture;

    bool m_RealTimeExecution = false;
    uint32_t m_CurrentSample = 0;

    ThreadPool* m_ThreadPool;

   public:
    CPUDevice(Texture2D<uint8_t, 3>* texture);
    ~CPUDevice();

    void OnUpdate() override;
    void SettingsUI() override;

   private:
    void Execute();

    void ExecuteThreaded();
    void ExecuteSingle();
    void ExecuteThreadedRT();
    void ExecuteSingleRT();

    void AccumulateSection(uint32_t x, uint32_t y, uint32_t s, uint32_t width, uint32_t height, std::atomic<bool>& stop);
};

}  // namespace raytracing