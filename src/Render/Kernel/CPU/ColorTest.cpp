#include "ColorTest.h"

#include "imgui.h"

namespace raytracing {

ColorTestKernel::ColorTestKernel() : Kernel("Color Test") {
    RT_LOG("Learn Kernel Init");
}

ColorTestKernel::~ColorTestKernel() {}

Color ColorTestKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    return m_Color;
}

void ColorTestKernel::UI() {
    ImGui::SliderFloat3("Color", &m_Color[0], 0.0f, 1.0f);
}

}  // namespace raytracing