#include "ColorTest.h"

#include "imgui.h"

namespace raytracing {

ColorTestKernel::ColorTestKernel() : Kernel("Color Test") {
    RT_LOG("Learn Kernel Init");
}

ColorTestKernel::~ColorTestKernel() {}

glm::vec3 ColorTestKernel::Exec(Texture2D<uint8_t, 3>* texture, uint32_t x, uint32_t y, uint32_t s) {
    return m_Color;
}

void ColorTestKernel::UI() {
    ImGui::ColorEdit3("Color", &m_Color[0]);
}

}  // namespace raytracing