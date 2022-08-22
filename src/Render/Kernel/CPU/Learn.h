#pragma once

#include "Render/Kernel/Kernel.h"
#include "Window/Image.h"

namespace raytracing {

class LearnKernel : public Kernel {
   private:
    int m_Color[3] = {0, 0, 0};

   public:
    LearnKernel();
    ~LearnKernel();

    Pixel Exec(Image* image, uint32_t x, uint32_t y) override;
    void UI() override;
};

}  // namespace raytracing