#include "CircleTest.h"

#include "imgui.h"

namespace raytracing {

CircleTestKernel::CircleTestKernel() : Kernel("Circle Test") {
    RT_LOG("Circle Test Kernel Init");
}

CircleTestKernel::~CircleTestKernel() {}

Pixel CircleTestKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    x -= image->GetWidth() / 2;
    y -= image->GetHeight() / 2;
    if (std::sqrt((x * x) + (y * y)) <= (uint32_t)m_CircleRadius) {
        return Pixel{0, 255, 0};
    }
    return Pixel{255, 0, 0};
}

void CircleTestKernel::UI() {
    ImGui::SliderInt("Circle Radius", &m_CircleRadius, 0, 100);
}

}  // namespace raytracing