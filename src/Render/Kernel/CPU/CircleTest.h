#pragma once

#include "Render/Kernel/Kernel.h"

namespace raytracing {

class CircleTestKernel : public Kernel {
   private:
    int m_CircleRadius = 25;

   public:
    CircleTestKernel();
    ~CircleTestKernel();

    glm::vec3 Exec(Texture2D<uint8_t, 3>* texture, uint32_t x, uint32_t y, uint32_t s) override;
    void UI() override;
};

}  // namespace raytracing