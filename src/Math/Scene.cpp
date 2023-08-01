#include "Scene.h"

#include "Window/UI.h"

namespace raytracing {

Scene::Scene(std::string name, glm::vec3 position, S<Camera> camera) : Object(name, position, nullptr), m_Camera(camera) {}

Scene::~Scene() {
    m_Objects.clear();
}

bool Scene::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) {
    RT_PROFILE_FUNC;

    HitResult temp_hr;
    bool any_hit = false;
    float closest = t_max;
    for (auto&& object : m_Objects) {
        if (object->Hit(ray, t_min, closest, temp_hr)) {
            any_hit = true;
            closest = temp_hr.t;
            hit = temp_hr;
        }
    }
    return any_hit;
}

SceneManager::SceneManager() {
    S<Scene> first_scene = FirstScene();
    S<Scene> mat_test_scene = MaterialTestScene();
    S<Scene> lens_test_scene = LensTestScene();
    S<Scene> random_scene = RandomLargeScene();

    m_SceneList.emplace_back(first_scene);
    m_SceneList.emplace_back(mat_test_scene);
    m_SceneList.emplace_back(lens_test_scene);
    m_SceneList.emplace_back(random_scene);

    m_CurrentScene = mat_test_scene;
    m_CurrentSceneIndex = 1;
};

void SceneManager::UI() {
    SceneComboUI();
    const S<Camera>& camera = m_CurrentScene->GetCamera();
    UI::InputFloat3("Camera Position", &camera->position.x);
    UI::InputFloat3("Camera Direction", &camera->direction.x);
    UI::SliderFloat("Camera Speed", &camera->speed, 0.1f, 1.0f);
    UI::InputFloat("Camera Vertical FOV", &camera->vfov);
    UI::Checkbox("Camera Lens", &camera->useLens);
    UI::SliderFloat("Camera Aperture", &camera->aperture, 0.0f, 4.0f);
    UI::SliderFloat("Camera Focus Distance", &camera->focus_dist, 0.0f, 20.0f);
    // for (Object* object : m_Scene->GetObjects()) {
    //     ImGui::SliderFloat3(object->name.c_str(), &object->position.x, -10.0f,
    //                         10.0f);
    // }
}

void SceneManager::SceneComboUI() {
    if (ImGui::BeginCombo("Scene", m_CurrentScene->name.c_str())) {
        for (int s = 0; s < m_SceneList.size(); s++) {
            S<Scene> scene = m_SceneList[s];
            const bool is_selected = (s == m_CurrentSceneIndex);
            if (UI::Selectable(scene->name.c_str(), is_selected)) {
                m_CurrentScene = scene;
                m_CurrentSceneIndex = s;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

S<Scene> SceneManager::FirstScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->position = glm::vec3(0.0f, 0.0f, -2.5f);
    camera->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    S<Scene> scene = CreateS<Scene>("First Scene", glm::vec3(0.0f), camera);
    scene->Add<Sphere>("Sphere 1", glm::vec3(0, 0, 0), CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f);
    scene->Add<Sphere>("Sphere 2", glm::vec3(2.0, 0, 0), CreateS<Metal>(glm::vec3(0.8f), 0.3f), 1.0f);
    scene->Add<Sphere>("Sphere 3", glm::vec3(-2.0, 0, 0), CreateS<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f), 1.0f);
    // scene->Add<Box>("Box", glm::vec3(5, 0, 0), CreateS<Lambertian>(glm::vec3(0.5f)), glm::vec3(1, 1, 1)));
    scene->Add<Plane>("Plane", glm::vec3(0, -1, 0), CreateS<Lambertian>(glm::vec3(0.8, 0.8, 0.0f)), glm::vec3(0, 1, 0));
    return scene;
}

S<Scene> SceneManager::MaterialTestScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->position = glm::vec3(2.0f, 4.0f, 5.0f);
    camera->direction = glm::vec3(-2.0f, -4.0f, -5.0f);
    S<Scene> scene = CreateS<Scene>("Material Test", glm::vec3(0.0f), camera);
    scene->Add<Sphere>("Ground", glm::vec3(0, -1000.0f, 0), CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f);
    scene->Add<Box>("Red Cube", glm::vec3(2.0f, 1.0f, -2.0f), CreateS<Lambertian>(glm::vec3(0.5f, 0.0f, 0.0f)), glm::vec3(1.0f));
    scene->Add<Sphere>("Green Sphere", glm::vec3(-2.0f, 1.0f, 2.0f), CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.0f)), 1.0f);
    scene->Add<Sphere>("Metal Sphere", glm::vec3(2.0f, 1.0f, 2.0f), CreateS<Metal>(glm::vec3(0.6f), 0.5f), 1.0f);
    scene->Add<Sphere>("Purple Sphere", glm::vec3(-2.0f, 1.0f, -2.0f), CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.5f)), 1.0f);
    scene->Add<Sphere>("Glass Sphere", glm::vec3(0.0f, 1.0f, 0.0f), CreateS<Dielectric>(1.5f), 1.0f);
    return scene;
}

S<Scene> SceneManager::LensTestScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->position = glm::vec3(0.0f, 0.0f, -2.5f);
    camera->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    S<Scene> scene = CreateS<Scene>("Lens Test", glm::vec3(0.0f), camera);
    scene->Add<Sphere>("Sphere 1", glm::vec3(0, 0, 0), CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f);
    scene->Add<Sphere>("Ground", glm::vec3(0, -1001.0f, 0), CreateS<Lambertian>(glm::vec3(1.0f)), 1000.0f);
    return scene;
}

S<Scene> SceneManager::RandomLargeScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->position = glm::vec3(13.0f, 2.0f, 3.0f);
    camera->direction = glm::vec3(-13.0f, -2.0f, -3.0f);
    S<Scene> scene = CreateS<Scene>("Random Large", glm::vec3(0.0f), camera);

    S<Lambertian> ground_material = CreateS<Lambertian>(glm::vec3(0.5f));
    // scene->Add(CreateU<Plane>("Ground", glm::vec3(0.0f), CreateS<Lambertian>(glm::vec3(0.5f)), glm::vec3(0, 1, 0)));
    scene->Add<Sphere>("Ground", glm::vec3(0, -1000.0f, 0), CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f);

    uint32_t seed = 12395u;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = FastRandom::Float(seed);
            glm::vec3 center = glm::vec3(a + 0.9f * FastRandom::Float(seed), 0.2f, b + 0.9f * FastRandom::Float(seed));
            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                S<Material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = FastRandom::Vec3(seed) * FastRandom::Vec3(seed);
                    sphere_material = CreateS<Lambertian>(albedo);
                    scene->Add<Sphere>("Sphere", center, sphere_material, 0.2f);
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = FastRandom::Vec3(seed, 0.5f, 1.0f);
                    auto fuzz = FastRandom::Float(seed, 0.0f, 0.5f);
                    sphere_material = CreateS<Metal>(albedo, fuzz);
                    scene->Add<Sphere>("Sphere", center, sphere_material, 0.2f);
                } else {
                    // glass
                    sphere_material = CreateS<Dielectric>(1.5);
                    scene->Add<Sphere>("Sphere", center, sphere_material, 0.2f);
                }
            }
        }
    }

    S<Dielectric> material1 = CreateS<Dielectric>(1.5);
    scene->Add<Sphere>("Sphere", glm::vec3(0.0f, 1.0f, 0.0f), material1, 1.0f);

    S<Lambertian> material2 = CreateS<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
    scene->Add<Sphere>("Sphere", glm::vec3(-4.0f, 1.0f, 0.0f), material2, 1.0f);

    S<Metal> material3 = CreateS<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f);
    scene->Add<Sphere>("Sphere", glm::vec3(4.0f, 1.0f, 0.0f), material3, 1.0f);

    return scene;
}

}  // namespace raytracing