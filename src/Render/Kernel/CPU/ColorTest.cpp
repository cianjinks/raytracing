#include "ColorTest.h"

#include "Window/UI.h"

namespace raytracing {

ColorTestKernel::ColorTestKernel() : Kernel("Color Test") {
    RT_LOG("Learn Kernel Init");
}

ColorTestKernel::~ColorTestKernel() {}

glm::vec3 ColorTestKernel::Exec(Texture2D3u8* texture, uint32_t x, uint32_t y, uint32_t s) {
    return m_Color;
}

void ColorTestKernel::UI() {
    UI::ColorEdit3("Color", &m_Color[0]);
}

}  // namespace raytracing