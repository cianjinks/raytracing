#include "Learn.h"

#include "Window/UI.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") {
    RT_PROFILE_FUNC_N("Learn Kernel Init");
    m_SceneManager = CreateU<SceneManager>();
}

LearnKernel::~LearnKernel() {}

glm::vec3 LearnKernel::Exec(const S<Texture2D3u8>& texture, uint32_t x, uint32_t y, uint32_t s) {
    RT_PROFILE_FUNC;

    uint32_t seed = (x + 1) * (y + 1) * (s + 1);

    const Scene& scene = m_SceneManager->GetScene();
    const Camera& camera = scene.GetCamera();

    /* Slight variation across samples for anti-aliasing. */
    float fx = float(x) + FastRandom::Float(seed, -1.0f, 1.0f);
    float fy = float(y) + FastRandom::Float(seed, -1.0f, 1.0f);
    float fs = ((fx * 2.0f) / float(texture->GetWidth())) - 1.0f;  /* -1 -> 1 */
    float ft = ((fy * 2.0f) / float(texture->GetHeight())) - 1.0f; /* -1 -> 1 */

    Ray ray = camera.GetRay(fs, ft, texture->GetAspectRatio(), seed);

    return RayColor(scene, ray, m_MaxBounces, seed);
}

bool LearnKernel::OnUpdate() {
    return m_SceneManager->GetScene().GetCamera().OnUpdate();
}

void LearnKernel::OnEvent(Event& event) {
    m_SceneManager->GetScene().GetCamera().OnEvent(event);
}

void LearnKernel::UI() {
    RT_PROFILE_FUNC;

    UI::SliderInt("Max Bounces", (int*)&m_MaxBounces, 0, 50);
    m_SceneManager->UI();
}

glm::vec3 LearnKernel::RayColor(const Scene& scene, const Ray& ray, uint32_t depth, uint32_t& seed) {
    RT_PROFILE_FUNC;

    HitResult result;

    if (depth <= 0) return glm::vec3(0.0f);

    if (scene.Hit(ray, 0.001f, Constant::FInfinity, result)) {
        seed += depth;

        Ray scattered;
        glm::vec3 attenuation;
        glm::vec3 emitted = result.material->emitted();
        if (result.material->scatter(ray, seed, result, attenuation, scattered)) {
            return emitted + attenuation * RayColor(scene, scattered, depth - 1, seed);
        }
        return emitted;
    }

    return scene.GetSkyColor();
}

}  // namespace raytracing