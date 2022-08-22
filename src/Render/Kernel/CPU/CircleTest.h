#pragma once

#include "Render/Kernel/Kernel.h"
#include "Window/Image.h"

namespace raytracing {

class CircleTestKernel : public Kernel {
   private:
    int m_CircleRadius = 25;

   public:
    CircleTestKernel();
    ~CircleTestKernel();

    Pixel Exec(Image* image, uint32_t x, uint32_t y) override;
    void UI() override;
};

}  // namespace raytracing