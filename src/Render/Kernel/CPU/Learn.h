#pragma once

#include <glm/glm.hpp>

#include "Render/Kernel/Kernel.h"
#include "Window/Image.h"

namespace raytracing {

class LearnKernel : public Kernel {
   private:
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, -1.0f};
    glm::vec3 m_CameraDirection = {0.0f, 0.0f, 1.0f};

   public:
    LearnKernel();
    ~LearnKernel();

    Pixel Exec(Image* image, uint32_t x, uint32_t y) override;
    void UI() override;
};

}  // namespace raytracing