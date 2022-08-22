#include "ColorTest.h"

#include "imgui.h"

namespace raytracing {

ColorTestKernel::ColorTestKernel() : Kernel("Color Test") {
    RT_LOG("Learn Kernel Init");
}

ColorTestKernel::~ColorTestKernel() {}

Pixel ColorTestKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    return Pixel{(uint8_t)m_Color[0], (uint8_t)m_Color[1], (uint8_t)m_Color[2]};
}

void ColorTestKernel::UI() { ImGui::SliderInt3("Color", m_Color, 0, 255); }

}  // namespace raytracing