#pragma once

#include <glm/glm.hpp>

#include "Math/Constant.h"
#include "Math/Material.h"
#include "Math/Object.h"
#include "Render/Kernel/Kernel.h"

namespace raytracing {

class LearnKernel : public Kernel {
   private:
    Scene* m_Scene;
    Camera* m_Camera;
    bool m_UseLens = false;
    uint32_t m_MaxBounces = 10;

   public:
    LearnKernel();
    ~LearnKernel();

    glm::vec3 Exec(Texture2D<uint8_t, 3>* texture, uint32_t x, uint32_t y, uint32_t s) override;
    void UI() override;

   private:
    glm::vec3 RayColor(const Ray& ray, uint32_t depth, uint32_t& seed);

    void FirstScene();
    void TestMaterialScene();
    void TestLensScene();
    void RandomizeScene();
};

}  // namespace raytracing