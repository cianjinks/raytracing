#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") {
    m_Camera = new Camera({0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 1.0f});
    m_Scene = new Scene("Test Scene", {0, 0, 0});
    m_Scene->Add(new Sphere("Sphere 1", {0, 0, 0}, nullptr, 1.0f));
    // m_Scene->Add(new Box("Box", {5, 0, 0}, {1, 5, 1}));
    m_Scene->Add(new Plane("Plane", {0, -1, 0}, nullptr, {0, 1, 0}));
    RT_LOG("Learn Kernel Init");
}

LearnKernel::~LearnKernel() {
    delete m_Scene;
    delete m_Camera;
}

Color LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    /* Slight variation across samples for anti-aliasing. */
    float fx = float(x) + Random::Float(0.0f, 1.0f);
    float fy = float(y) + Random::Float(0.0f, 1.0f);
    float u =
        ((fx * (2.0f * image->GetAspectRatio())) / float(image->GetWidth())) -
        image->GetAspectRatio();
    float v = ((fy * 2.0f) / float(image->GetHeight())) - 1.0f;

    Ray ray;
    ray.origin = m_Camera->position;
    ray.direction = m_Camera->direction + glm::vec3(u, v, 0.0f);

    return RayColor(ray, m_MaxBounces);
}

void LearnKernel::UI() {
    ImGui::SliderInt("Max Bounces", (int*)&m_MaxBounces, 0, 50);
    ImGui::SliderFloat3("Camera Position", &m_Camera->position.x, -10.0f,
                        10.0f);
    ImGui::SliderFloat3("Camera Direction", &m_Camera->direction.x, -1.0f,
                        1.0f);
    for (Object* object : m_Scene->GetObjects()) {
        ImGui::SliderFloat3(object->name.c_str(), &object->position.x, -10.0f,
                            10.0f);
    }
}

Color LearnKernel::RayColor(const Ray& ray, int depth) {
    HitResult result;

    if (depth <= 0) return {0.0f, 0.0f, 0.0f};

    if (m_Scene->Hit(ray, Constant::FMin, Constant::FInfinity, result)) {
        Ray bounce_ray;
        bounce_ray.origin = result.position;
        /* 2 different formulas for diffuse reflection. */
        // glm::vec3 target = result.position + Random::InHemisphere(result.normal);
        glm::vec3 target = result.position + result.normal + glm::normalize(Random::InSphere());
        bounce_ray.direction = target - result.position;
        return 0.5f * RayColor(bounce_ray, depth - 1);
    }

    glm::vec3 unit = glm::normalize(ray.direction);
    float t = 0.5f * (unit.y + 1.0f);
    return (1.0f - t) * Color(1.0f) + t * Color(0.5f, 0.7f, 1.0f);
}

}  // namespace raytracing