#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") { RT_LOG("Learn Kernel Init"); }

LearnKernel::~LearnKernel() {}

Pixel LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    uint8_t v = std::rand() % 256;
    return Pixel{v, v, v};
}

void LearnKernel::UI() {}

}  // namespace raytracing