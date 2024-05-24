#pragma once

#include "RenderDevice.h"
#include "Image.h"
#include "Util/Thread.h"

namespace raytracing {

class CPUDevice : public RenderDevice {
   private:
    /* Execution settings */
    uint32_t m_NumSamples = 50;
    bool m_Multithreaded = true;
    uint32_t m_NumThreads = ThreadPool::s_MaxThreadCount - 1;
    uint32_t m_NumTilesX = 4;
    uint32_t m_NumTilesY = 4;

    /* Execution targets */
    S<Image2D3u8> m_Texture;
    U<Image2D3f> m_AccumulationBuffer;

    /* Execution utilities */
    U<ThreadPool> m_ThreadPool;
    Timer m_Timer;

    /* Real time execution flags */
    bool m_RealTimeExecution = false;
    uint32_t m_CurrentSample = 0;

    /* Once-off execution flags */
    bool m_IsExecuting = false;

   public:
    CPUDevice();
    ~CPUDevice();

    void OnUpdate() override;
    void OnEvent(Event& event) override{};
    void SettingsUI() override;

    void Dirty() override;

   private:
    bool Execute();

    void ExecuteThreaded();
    void ExecuteSingle();
    void ExecuteThreadedRT();
    void ExecuteSingleRT();

    void AccumulateSection(uint32_t x, uint32_t y, uint32_t s, uint32_t width, uint32_t height, std::atomic<bool>& stop);
};

}  // namespace raytracing