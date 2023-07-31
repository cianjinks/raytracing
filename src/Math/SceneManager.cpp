#include "SceneManager.h"

#include "Window/UI.h"

namespace raytracing {

SceneManager::SceneManager() {
    /* FIRST SCENE */
    Camera camera({0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 0.2f, 90.0f, 2.0f, 2.5f);
    Scene* first_scene = new Scene("First Scene", {0, 0, 0}, camera);
    first_scene->Add(new Sphere("Sphere 1", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f));
    first_scene->Add(new Sphere("Sphere 2", {2.0, 0, 0}, CreateS<Metal>(glm::vec3(0.8f), 0.3f), 1.0f));
    first_scene->Add(new Sphere("Sphere 3", {-2.0, 0, 0}, CreateS<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f), 1.0f));
    // m_Scene->Add(new Box("Box", {5, 0, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), {1, 1, 1}));
    first_scene->Add(new Plane("Plane", {0, -1, 0}, CreateS<Lambertian>(glm::vec3(0.8, 0.8, 0.0f)), {0, 1, 0}));
    /****************/

    /* MATERIAL TEST SCENE */
    Camera camera2({2.0f, 4.0f, 5.0f}, {-2.0f, -4.0f, -5.0f}, {0.0f, 1.0f, 0.0f}, 0.2f, 90.0f, 2.0f, 2.5f);
    Scene* mat_test_scene = new Scene("Material Test", {0, 0, 0}, camera2);
    mat_test_scene->Add(new Sphere("Ground", {0, -1000.0f, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f));
    mat_test_scene->Add(new Box("Red Cube", {2.0f, 1.0f, -2.0f}, CreateS<Lambertian>(glm::vec3(0.5f, 0.0f, 0.0f)), glm::vec3(1.0f)));
    mat_test_scene->Add(new Sphere("Green Sphere", {-2.0f, 1.0f, 2.0f}, CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.0f)), 1.0f));
    mat_test_scene->Add(new Sphere("Metal Sphere", {2.0f, 1.0f, 2.0f}, CreateS<Metal>(glm::vec3(0.6f), 0.5f), 1.0f));
    mat_test_scene->Add(new Sphere("Purple Sphere", {-2.0f, 1.0f, -2.0f}, CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.5f)), 1.0f));
    mat_test_scene->Add(new Sphere("Glass Sphere", {0.0f, 1.0f, 0.0f}, CreateS<Dielectric>(1.5f), 1.0f));
    /****************/

    /* LENS TEST SCENE */
    Camera camera3({0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 0.2f, 90.0f, 2.0f, 2.5f);
    Scene* lens_test_scene = new Scene("Lens Test", {0, 0, 0}, camera3);
    lens_test_scene->Add(new Sphere("Sphere 1", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f));
    lens_test_scene->Add(new Sphere("Ground", {0, -1001.0f, 0}, CreateS<Lambertian>(glm::vec3(1.0f)), 1000.0f));
    /****************/

    /* RANDOM LARGE SCENE */
    Camera camera4({13.0f, 2.0f, 3.0f}, {-13.0f, -2.0f, -3.0f}, {0.0f, 1.0f, 0.0f}, 0.2f, 20.0f, 0.1f, 10.0f);
    Scene* random_scene = new Scene("Random Large", {0, 0, 0}, camera4);

    S<Lambertian> ground_material = CreateS<Lambertian>(glm::vec3(0.5f));
    // m_Scene->Add(new Plane("Ground", {0, 0, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), {0, 1, 0}));
    random_scene->Add(new Sphere("Ground", {0, -1000.0f, 0}, CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f));

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
                    random_scene->Add(new Sphere("Sphere", center, sphere_material, 0.2f));
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = FastRandom::Vec3(seed, 0.5f, 1.0f);
                    auto fuzz = FastRandom::Float(seed, 0.0f, 0.5f);
                    sphere_material = CreateS<Metal>(albedo, fuzz);
                    random_scene->Add(new Sphere("Sphere", center, sphere_material, 0.2f));
                } else {
                    // glass
                    sphere_material = CreateS<Dielectric>(1.5);
                    random_scene->Add(new Sphere("Sphere", center, sphere_material, 0.2f));
                }
            }
        }
    }

    S<Dielectric> material1 = CreateS<Dielectric>(1.5);
    random_scene->Add(new Sphere("Sphere", glm::vec3(0.0f, 1.0f, 0.0f), material1, 1.0f));

    S<Lambertian> material2 = CreateS<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
    random_scene->Add(new Sphere("Sphere", glm::vec3(-4.0f, 1.0f, 0.0f), material2, 1.0f));

    S<Metal> material3 = CreateS<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f);
    random_scene->Add(new Sphere("Sphere", glm::vec3(4.0f, 1.0f, 0.0f), material3, 1.0f));
    /****************/

    m_SceneList.emplace_back(first_scene);
    m_SceneList.emplace_back(mat_test_scene);
    m_SceneList.emplace_back(lens_test_scene);
    m_SceneList.emplace_back(random_scene);

    m_CurrentScene = mat_test_scene;
    m_CurrentSceneIndex = 1;
};

void SceneManager::UI() {
    SceneComboUI();
    Camera& camera = m_CurrentScene->GetCamera();
    UI::InputFloat3("Camera Position", &camera.position.x);
    UI::InputFloat3("Camera Direction", &camera.direction.x);
    UI::SliderFloat("Camera Speed", &camera.speed, 0.1f, 1.0f);
    UI::InputFloat("Camera Vertical FOV", &camera.vfov);
    UI::Checkbox("Camera Lens", &camera.useLens);
    UI::SliderFloat("Camera Aperture", &camera.aperture, 0.0f, 4.0f);
    UI::SliderFloat("Camera Focus Distance", &camera.focus_dist, 0.0f, 20.0f);
    // for (Object* object : m_Scene->GetObjects()) {
    //     ImGui::SliderFloat3(object->name.c_str(), &object->position.x, -10.0f,
    //                         10.0f);
    // }
}

void SceneManager::SceneComboUI() {
    if (ImGui::BeginCombo("Scene", m_CurrentScene->name.c_str())) {
        for (int s = 0; s < m_SceneList.size(); s++) {
            Scene* scene = m_SceneList[s];
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

}