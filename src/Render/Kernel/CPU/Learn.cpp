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

    float view_plane_height = 2.0f * glm::tan(glm::radians(camera.vfov) / 2.0f);
    float view_plane_width = texture->GetAspectRatio() * view_plane_height;

    glm::vec3 u = glm::cross(glm::normalize(camera.direction), camera.up);
    glm::vec3 v = glm::cross(u, glm::normalize(camera.direction));

    glm::vec3 vertical = view_plane_height * v;
    glm::vec3 horizontal = view_plane_width * u;

    /* Camera lens calculations. */
    glm::vec3 cd = glm::normalize(camera.direction);
    glm::vec3 offset = glm::vec3(0.0f);
    if (camera.useLens) {
        /* Offset camera position by point in a random disk oriented along camera view plane. */
        /* This simulates shooting rays from a circular lens. */
        /* TODO: My aperture seems to be off by a factor of 2. Maybe this divide by 2 is wrong because of -1 to 1 range? */
        glm::vec3 random_disk = (camera.aperture / 2.0f) * FastRandom::InUnitDisk(seed);
        offset = (u * random_disk.x) + (v * random_disk.y);

        /* Scale camera direction by focus_dist to push view plane farther away. */
        /* We also need to scale vertical / horizontal otherwise the view will be zoomed. */
        vertical = camera.focus_dist * vertical;
        horizontal = camera.focus_dist * horizontal;
        cd = camera.focus_dist * cd;
    }

    Ray ray;
    ray.origin = camera.position + offset;
    /* Half vertical and horizontal because camera.direction is in the centre of the view plane, but they were calculated with the full width and height. */
    ray.direction = cd + ((horizontal / 2.0f) * fs) + ((vertical / 2.0f) * ft) - offset;

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

    if (depth <= 0) return {0.0f, 0.0f, 0.0f};

    if (scene.Hit(ray, 0.001f, Constant::FInfinity, result)) {
        seed += depth;

        Ray scattered;
        glm::vec3 attenuation;
        if (result.material->scatter(ray, seed, result, attenuation, scattered)) {
            return attenuation * RayColor(scene, scattered, depth - 1, seed);
        }
        return glm::vec3(0.0f);
    }

    glm::vec3 unit = glm::normalize(ray.direction);
    float t = 0.5f * (unit.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

}  // namespace raytracing