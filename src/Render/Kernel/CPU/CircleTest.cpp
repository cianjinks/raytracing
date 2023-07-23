#include "CircleTest.h"

#include "imgui.h"

namespace raytracing {

CircleTestKernel::CircleTestKernel() : Kernel("Circle Test") {
    RT_LOG("Circle Test Kernel Init");
}

CircleTestKernel::~CircleTestKernel() {}

glm::vec3 CircleTestKernel::Exec(Texture2D<uint8_t, 3>* texture, uint32_t x, uint32_t y, uint32_t s) {
    x -= texture->GetWidth() / 2;
    y -= texture->GetHeight() / 2;
    if (std::sqrt((x * x) + (y * y)) <= (uint32_t)m_CircleRadius) {
        return {0, 1, 0};
    }
    return {1, 0, 0};
}

void CircleTestKernel::UI() {
    ImGui::SliderInt("Circle Radius", &m_CircleRadius, 0, 100);
}

}  // namespace raytracing