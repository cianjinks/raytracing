#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") {
    m_Camera = new Camera({0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f});
    m_Scene = new Scene("Test Scene", {0, 0, 0});
    m_Scene->Add(new Sphere("Sphere 1", {0, 0, 0}, 1.0f));
    m_Scene->Add(new Box("Box", {5, 0, 0}, {1, 5, 1}));
    m_Scene->Add(new Plane("Plane", {0, -1, 0}, {0, 1, 0}));
    RT_LOG("Learn Kernel Init");
}

LearnKernel::~LearnKernel() {
    delete m_Scene;
    delete m_Camera;
}

Color LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    float u = ((float(x) * (2.0f * image->GetAspectRatio())) /
               float(image->GetWidth())) -
              image->GetAspectRatio();
    float v = ((float(y) * 2.0f) / float(image->GetHeight())) - 1.0f;

    Ray ray;
    ray.origin = m_Camera->position;
    ray.direction = m_Camera->direction + glm::vec3(u, v, 0.0f);

    HitResult result;
    if (m_Scene->Hit(ray, Constant::FMin, Constant::FInfinity, result)) {
        return {glm::abs(result.normal)};
    }
    return {0.5, 0.7, 1.0};
}

void LearnKernel::UI() {
    ImGui::SliderFloat3("Camera Position", &m_Camera->position.x, -10.0f,
                        10.0f);
    ImGui::SliderFloat3("Camera Direction", &m_Camera->direction.x, -1.0f,
                        1.0f);
    for (Object* object : m_Scene->GetObjects()) {
        ImGui::SliderFloat3(object->name.c_str(), &object->position.x, -10.0f,
                            10.0f);
    }
}

}  // namespace raytracing