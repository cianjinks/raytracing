#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") {
    m_Scene = new Scene();
    m_Scene->Add(new Sphere({0, 0, 0}, 1.0f));
    m_Scene->Add(new Sphere({0, -101, 0}, 100.0f));
    RT_LOG("Learn Kernel Init");
}

LearnKernel::~LearnKernel() { delete m_Scene; }

Pixel LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    float u = ((float(x) * (2.0f * image->GetAspectRatio())) /
               float(image->GetWidth())) -
              image->GetAspectRatio();
    float v = ((float(y) * 2.0f) / float(image->GetHeight())) - 1.0f;

    Ray ray;
    ray.origin = m_CameraPosition;
    ray.direction = m_CameraDirection + glm::vec3(u, v, 0.0f);

    HitResult result;
    if (m_Scene->Hit(ray, 0, Constant::Infinity, result)) {
        return {glm::abs(result.normal)};
    }
    return {0, 0, 255};
}

void LearnKernel::UI() {
    ImGui::SliderFloat("Camera Z", &m_CameraPosition.z, 0.0f, -10.0f);
}

}  // namespace raytracing