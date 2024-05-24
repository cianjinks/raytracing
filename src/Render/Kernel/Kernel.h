#pragma once

#include "Render/Image.h"
#include "Window/Event.h"

/**
 * @brief Kernel
 * Not to be confused with OpenCL kernels.
 * A Kernel represents an algorithm to be ran on a device. The algorithm updates
 * a Image2D in the device manager. Ths allows me to have multiple algorithms for multiple devices for
 * learning.
 */

namespace raytracing {

class Kernel {
   public:
    std::string Name;

   public:
    Kernel(std::string name);
    virtual ~Kernel() = default;

    virtual glm::vec3 Exec(const S<Image2D3u8>& texture, uint32_t x, uint32_t y, uint32_t s, uint32_t& seed) = 0;

    /* A kernel may want to do something every frame. Return true if it did. */
    virtual bool OnUpdate() = 0;
    virtual void OnEvent(Event& event) = 0;
    virtual void UI() = 0;
};

class KernelLibrary {
   private:
    std::vector<S<Kernel>> m_KernelList;
    S<Kernel> m_CurrentKernel = nullptr;
    uint32_t m_CurrentKernelIndex = 0;

   public:
    KernelLibrary();
    ~KernelLibrary();

    void DeviceComboUI();

    void AddKernel(S<Kernel> kernel);
    void SetCurrentKernel(uint32_t index);
    S<Kernel> GetCurrentKernel() const { return m_CurrentKernel; }
};

}  // namespace raytracing