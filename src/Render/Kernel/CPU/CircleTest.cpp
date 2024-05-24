#include "CircleTest.h"

#include "Window/UI.h"

namespace raytracing {

CircleTestKernel::CircleTestKernel() : Kernel("Circle Test") {
    RT_LOG("Circle Test Kernel Init");
}

CircleTestKernel::~CircleTestKernel() {}

glm::vec3 CircleTestKernel::Exec(const S<Image2D3u8>& texture, uint32_t x, uint32_t y, uint32_t s, uint32_t& seed) {
    x -= texture->GetWidth() / 2;
    y -= texture->GetHeight() / 2;
    if (std::sqrt((x * x) + (y * y)) <= (uint32_t)m_CircleRadius) {
        return {0, 1, 0};
    }
    return {1, 0, 0};
}

void CircleTestKernel::UI() {
    UI::SliderInt("Circle Radius", &m_CircleRadius, 0, 100);
}

}  // namespace raytracing