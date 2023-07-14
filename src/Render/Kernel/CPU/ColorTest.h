#pragma once

#include "Render/Kernel/Kernel.h"
#include "Window/Image.h"

namespace raytracing {

class ColorTestKernel : public Kernel {
   private:
    Color m_Color = {0, 0, 0};

   public:
    ColorTestKernel();
    ~ColorTestKernel();

    Color Exec(Image* image, uint32_t x, uint32_t y, uint32_t s) override;
    void UI() override;
};

}  // namespace raytracing