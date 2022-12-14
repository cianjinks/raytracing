#pragma once

#include "Window/Image.h"

/**
 * @brief Kernel
 * Not to be confused with OpenCL kernels.
 * A Kernel represents an algorithm to be ran on a device. The algorithm updates
 * an Image. Ths allows me to have multiple algorithms for multiple devices for
 * learning.
 */

namespace raytracing {

class Kernel {
   public:
    std::string Name;

   public:
    Kernel(std::string name);
    virtual ~Kernel() = default;

    virtual Color Exec(Image* image, uint32_t x, uint32_t y) = 0;
    virtual void UI() = 0;
};

class KernelLibrary {
   private:
    std::vector<Kernel*> m_KernelList;
    Kernel* m_CurrentKernel = nullptr;
    uint32_t m_CurrentKernelIndex = 0;

   public:
    KernelLibrary();
    ~KernelLibrary();

    void DeviceComboUI();

    void AddKernel(Kernel* kernel);
    void SetCurrentKernel(uint32_t index);
    Kernel* GetCurrentKernel() const { return m_CurrentKernel; }
};

}  // namespace raytracing