#include "Learn.h"

#include "Math/Intersection.h"
#include "Math/Object.h"
#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") { RT_LOG("Learn Kernel Init"); }

LearnKernel::~LearnKernel() {}

Pixel LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    float u = ((float(x) * (2.0f * image->GetAspectRatio())) /
               float(image->GetWidth())) -
              image->GetAspectRatio();
    float v = ((float(y) * 2.0f) / float(image->GetHeight())) - 1.0f;

    Sphere sphere;
    sphere.position = {0, 0, 0};
    sphere.radius = 1.0f;

    Ray ray;
    ray.origin = m_CameraPosition;
    ray.direction =
        m_CameraDirection +
        glm::vec3(u, v, 0.0f);  // glm::vec3(u, v, 0.0f) - m_CameraPosition;

    if (Intersection::RaySphere(ray, sphere)) {
        return {255, 0, 0};
    }
    return {0, 0, 255};
}

void LearnKernel::UI() {
    ImGui::SliderFloat("Camera Z", &m_CameraPosition.z, 0.0f, -5.0f);
}

}  // namespace raytracing