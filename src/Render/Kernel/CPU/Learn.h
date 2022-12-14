#pragma once

#include <glm/glm.hpp>

#include "Math/Constant.h"
#include "Math/Object.h"
#include "Render/Kernel/Kernel.h"
#include "Window/Image.h"

namespace raytracing {

class LearnKernel : public Kernel {
   private:
    Scene* m_Scene;
    Camera* m_Camera;
    uint32_t m_MaxBounces = 5;

   public:
    LearnKernel();
    ~LearnKernel();

    Color Exec(Image* image, uint32_t x, uint32_t y) override;
    void UI() override;

   private:
    Color RayColor(const Ray& ray, int depth);
};

}  // namespace raytracing