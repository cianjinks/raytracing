#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") {
    m_Camera = new Camera({0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 2.0f, 2.5f);
    m_Scene = new Scene("Test Scene", {0, 0, 0});
    m_Scene->Add(new Sphere("Sphere 1", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f));
    m_Scene->Add(new Sphere("Sphere 2", {-2.0, 0, 0}, CreateS<Dielectric>(1.5f), 1.0f));
    m_Scene->Add(new Sphere("Sphere 3", {2.0, 0, 0}, CreateS<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f), 1.0f));
    // m_Scene->Add(new Box("Box", {5, 0, 0}, {1, 5, 1}));
    m_Scene->Add(new Plane("Plane", {0, -1, 0}, CreateS<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f)), {0, 1, 0}));
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
    float s =
        ((fx * (2.0f * image->GetAspectRatio())) / float(image->GetWidth())) -
        image->GetAspectRatio();
    float t = ((fy * 2.0f) / float(image->GetHeight())) - 1.0f;

    glm::vec3 u = glm::normalize(glm::cross(glm::normalize(m_Camera->direction), m_Camera->up));
    glm::vec3 v = glm::cross(u, glm::normalize(m_Camera->direction));

    /* Camera lens calculations. */
    glm::vec3 cd = m_Camera->direction;
    glm::vec3 offset = glm::vec3(0.0f);
    if (m_UseLens) {
        cd = m_Camera->direction * m_Camera->focus_dist;
        glm::vec3 rd = (m_Camera->aperture / 2.0f) * Random::InUnitDisk();
        offset = (u * rd.x) + (v * rd.y);
        s = s * m_Camera->focus_dist;
        t = t * m_Camera->focus_dist;
    }

    Ray ray;
    ray.origin = m_Camera->position + offset;
    ray.direction = cd + (u * s) + (v * t) - offset;

    return RayColor(ray, m_MaxBounces);
}

void LearnKernel::UI() {
    ImGui::SliderInt("Max Bounces", (int*)&m_MaxBounces, 0, 50);
    ImGui::InputFloat3("Camera Position", &m_Camera->position.x);
    ImGui::InputFloat3("Camera Direction", &m_Camera->direction.x);
    ImGui::Checkbox("Camera Lens", &m_UseLens);
    ImGui::InputFloat("Camera Aperture", &m_Camera->aperture);
    ImGui::InputFloat("Camera Focus Distance", &m_Camera->focus_dist);
    for (Object* object : m_Scene->GetObjects()) {
        ImGui::SliderFloat3(object->name.c_str(), &object->position.x, -10.0f,
                            10.0f);
    }
}

Color LearnKernel::RayColor(const Ray& ray, int depth) {
    HitResult result;

    if (depth <= 0) return {0.0f, 0.0f, 0.0f};

    if (m_Scene->Hit(ray, Constant::FMin, Constant::FInfinity, result)) {
        Ray scattered;
        glm::vec3 attenuation;
        if (result.material->scatter(ray, result, attenuation, scattered)) {
            return attenuation * RayColor(scattered, depth - 1);
        }
        return glm::vec3(0.0f);
    }

    glm::vec3 unit = glm::normalize(ray.direction);
    float t = 0.5f * (unit.y + 1.0f);
    return (1.0f - t) * Color(1.0f) + t * Color(0.5f, 0.7f, 1.0f);
}

}  // namespace raytracing