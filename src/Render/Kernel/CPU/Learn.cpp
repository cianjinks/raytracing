#include "Learn.h"

#include "imgui.h"

namespace raytracing {

LearnKernel::LearnKernel() : Kernel("Learn") {
    RT_PROFILE_FUNC_N("Learn Kernel Init");

    // m_Camera = new Camera({0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 2.0f, 2.5f);
    // m_Scene = new Scene("Test Scene", {0, 0, 0});
    // m_Scene->Add(new Sphere("Sphere 1", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f));
    // m_Scene->Add(new Sphere("Sphere 2", {2.0, 0, 0}, CreateS<Metal>(glm::vec3(0.8), 0.3f), 1.0f));
    // m_Scene->Add(new Sphere("Sphere 3", {-2.0, 0, 0}, CreateS<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f), 1.0f));
    // // m_Scene->Add(new Box("Box", {5, 0, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), {1, 1, 1}));
    // m_Scene->Add(new Plane("Plane", {0, -1, 0}, CreateS<Lambertian>(glm::vec3(0.8, 0.8, 0.0f)), {0, 1, 0}));

    m_Camera = new Camera({13.0f, 2.0f, 3.0f}, {-13.0f, -2.0f, -3.0f}, {0.0f, 1.0f, 0.0f}, 20.0f, 0.1f, 10.0f);
    m_Scene = new Scene("Test Scene", {0, 0, 0});
    RandomizeScene();

    // m_Camera = new Camera({2.0f, 4.0f, 5.0f}, {-2.0f, -4.0f, -5.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 2.0f, 2.5f);
    // m_Scene = new Scene("Test Scene", {0, 0, 0});
    // m_Scene->Add(new Sphere("Ground", {0, -1000.0f, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f));
    // m_Scene->Add(new Sphere("Red Sphere", {1.5f, 1.0f, -1.5f}, CreateS<Lambertian>(glm::vec3(0.5f, 0.0f, 0.0f)), 1.0f));
    // m_Scene->Add(new Sphere("Green Sphere", {-1.5f, 1.0f, 1.5f}, CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.0f)), 1.0f));
    // m_Scene->Add(new Sphere("Metal Sphere", {1.5f, 1.0f, 1.5f}, CreateS<Metal>(glm::vec3(0.6f), 0.5f), 1.0f));
    // m_Scene->Add(new Sphere("Purple Sphere", {-1.5f, 1.0f, -1.5f}, CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.5f)), 1.0f));

    // m_Camera = new Camera({0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 2.0f, 2.5f);
    // m_Scene = new Scene("Test Scene", {0, 0, 0});
    // m_Scene->Add(new Sphere("Sphere 1", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f));
    // m_Scene->Add(new Sphere("Ground", {0, -1001.0f, 0}, CreateS<Lambertian>(glm::vec3(1.0f)), 1000.0f));

    RT_LOG("Learn Kernel Init");
}

LearnKernel::~LearnKernel() {
    delete m_Scene;
    delete m_Camera;
}

void LearnKernel::RandomizeScene() {
    S<Lambertian> ground_material = CreateS<Lambertian>(glm::vec3(0.5f));
    // m_Scene->Add(new Plane("Ground", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), {0, 1, 0}));
    m_Scene->Add(new Sphere("Ground", {0, -1000.0f, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = Random::Float();
            glm::vec3 center = glm::vec3(a + 0.9f * Random::Float(), 0.2f, b + 0.9f * Random::Float());
            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                S<Material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = Random::Vec3() * Random::Vec3();
                    sphere_material = CreateS<Lambertian>(albedo);
                    m_Scene->Add(new Sphere("Sphere", center, sphere_material, 0.2f));
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = Random::Vec3(0.5f, 1.0f);
                    auto fuzz = Random::Float(0.0f, 0.5f);
                    sphere_material = CreateS<Metal>(albedo, fuzz);
                    m_Scene->Add(new Sphere("Sphere", center, sphere_material, 0.2f));
                } else {
                    // glass
                    sphere_material = CreateS<Dielectric>(1.5);
                    m_Scene->Add(new Sphere("Sphere", center, sphere_material, 0.2f));
                }
            }
        }
    }

    S<Dielectric> material1 = CreateS<Dielectric>(1.5);
    m_Scene->Add(new Sphere("Sphere", glm::vec3(0.0f, 1.0f, 0.0f), material1, 1.0f));

    S<Lambertian> material2 = CreateS<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
    m_Scene->Add(new Sphere("Sphere", glm::vec3(-4.0f, 1.0f, 0.0f), material2, 1.0f));

    S<Metal> material3 = CreateS<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f);
    m_Scene->Add(new Sphere("Sphere", glm::vec3(4.0f, 1.0f, 0.0f), material3, 1.0f));
}

Color LearnKernel::Exec(Image* image, uint32_t x, uint32_t y) {
    RT_PROFILE_FUNC;

    /* Slight variation across samples for anti-aliasing. */
    float fx = float(x) + Random::Float(-1.0f, 1.0f);
    float fy = float(y) + Random::Float(-1.0f, 1.0f);
    float s = ((fx * 2.0f) / float(image->GetWidth())) - 1.0f;  /* -1 -> 1 */
    float t = ((fy * 2.0f) / float(image->GetHeight())) - 1.0f; /* -1 -> 1 */

    float view_plane_height = 2.0f * glm::tan(glm::radians(m_Camera->vfov) / 2.0f);
    float view_plane_width = image->GetAspectRatio() * view_plane_height;

    glm::vec3 u = glm::cross(glm::normalize(m_Camera->direction), m_Camera->up);
    glm::vec3 v = glm::cross(u, glm::normalize(m_Camera->direction));

    glm::vec3 vertical = view_plane_height * v;
    glm::vec3 horizontal = view_plane_width * u;

    /* Camera lens calculations. */
    glm::vec3 cd = glm::normalize(m_Camera->direction);
    glm::vec3 offset = glm::vec3(0.0f);
    if (m_UseLens) {
        /* Offset camera position by point in a random disk oriented along camera view plane. */
        /* This simulates shooting rays from a circular lens. */
        glm::vec3 random_disk = (m_Camera->aperture / 2.0f) * Random::InUnitDisk();
        offset = (u * random_disk.x) + (v * random_disk.y);

        /* Scale camera direction by focus_dist to push view plane farther away. */
        /* We also need to scale vertical / horizontal otherwise the view will be zoomed. */
        vertical = m_Camera->focus_dist * vertical;
        horizontal = m_Camera->focus_dist * horizontal;
        cd = m_Camera->focus_dist * cd;
    }

    Ray ray;
    ray.origin = m_Camera->position + offset;
    /* Half vertical and horizontal because m_Camera->direction is in the centre of the view plane, but they were calculated with the full width and height. */
    ray.direction = cd + ((horizontal / 2.0f) * s) + ((vertical / 2.0f) * t) - offset;

    return RayColor(ray, m_MaxBounces);
}

void LearnKernel::UI() {
    RT_PROFILE_FUNC;

    ImGui::SliderInt("Max Bounces", (int*)&m_MaxBounces, 0, 50);
    ImGui::InputFloat3("Camera Position", &m_Camera->position.x);
    ImGui::InputFloat3("Camera Direction", &m_Camera->direction.x);
    ImGui::InputFloat("Camera Vertical FOV", &m_Camera->vfov);
    ImGui::Checkbox("Camera Lens", &m_UseLens);
    ImGui::InputFloat("Camera Aperture", &m_Camera->aperture);
    ImGui::InputFloat("Camera Focus Distance", &m_Camera->focus_dist);
    // for (Object* object : m_Scene->GetObjects()) {
    //     ImGui::SliderFloat3(object->name.c_str(), &object->position.x, -10.0f,
    //                         10.0f);
    // }
}

Color LearnKernel::RayColor(const Ray& ray, int depth) {
    RT_PROFILE_FUNC;

    HitResult result;

    if (depth <= 0) return {0.0f, 0.0f, 0.0f};

    if (m_Scene->Hit(ray, 0.001, Constant::FInfinity, result)) {
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