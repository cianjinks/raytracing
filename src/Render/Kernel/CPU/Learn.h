#pragma once

#include <glm/glm.hpp>

#include "Math/Camera.h"
#include "Math/Constant.h"
#include "Math/Material.h"
#include "Math/Object.h"
#include "Math/Scene.h"
#include "Render/Kernel/Kernel.h"

namespace raytracing {

class LearnKernel : public Kernel {
   private:
    U<SceneManager> m_SceneManager;
    uint32_t m_MaxBounces = 50;

   public:
    LearnKernel();
    ~LearnKernel();

    glm::vec3 Exec(const S<Image2D3u8>& texture, uint32_t x, uint32_t y, uint32_t s, uint32_t& seed) override;
    bool OnUpdate() override;
    void OnEvent(Event& event) override;
    void UI() override;

   private:
    glm::vec3 RayColor(const Scene& scene, const Ray& ray, uint32_t depth, uint32_t& seed);
};

}  // namespace raytracing