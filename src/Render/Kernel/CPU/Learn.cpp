#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") { RT_LOG("Learn Kernel Init"); }

LearnKernel::~LearnKernel() {}

Pixel LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    return Pixel{(uint8_t)m_Color[0], (uint8_t)m_Color[1], (uint8_t)m_Color[2]};
}

void LearnKernel::UI() { ImGui::SliderInt3("Color", m_Color, 0, 255); }

}  // namespace raytracing