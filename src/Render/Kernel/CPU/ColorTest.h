#pragma once

#include "Render/Kernel/Kernel.h"

namespace raytracing {

class ColorTestKernel : public Kernel {
   private:
    glm::vec3 m_Color = {0, 0, 0};

   public:
    ColorTestKernel();
    ~ColorTestKernel();

    glm::vec3 Exec(Texture2D3u8* texture, uint32_t x, uint32_t y, uint32_t s) override;
    bool OnUpdate() override { return false; }
    void UI() override;
};

}  // namespace raytracing