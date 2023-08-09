#include "Scene.h"

#include "Window/UI.h"

namespace raytracing {

Scene::Scene(std::string name, glm::vec3 position, S<Camera> camera) : Object(name, position, nullptr), m_Camera(camera) {}

Scene::~Scene() {
    m_Objects.clear();
}

bool Scene::Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const {
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
    S<Scene> light_test_scene = LightTestScene();
    S<Scene> cornell_box = CornellBox();

    m_SceneList.emplace_back(first_scene);
    m_SceneList.emplace_back(mat_test_scene);
    m_SceneList.emplace_back(lens_test_scene);
    m_SceneList.emplace_back(random_scene);
    m_SceneList.emplace_back(light_test_scene);
    m_SceneList.emplace_back(cornell_box);

    m_CurrentScene = cornell_box;
    m_CurrentSceneIndex = 5;
};

void SceneManager::UI() {
    if (ImGui::CollapsingHeader("Scene Options")) {
        SceneComboUI();
        UI::ColorEdit3("Sky Color", &m_CurrentScene->GetSkyColor().x);
    }
    if (ImGui::CollapsingHeader("Scene Objects")) {
        for (auto&& object : m_CurrentScene->GetObjects()) {
            // object->UI();
            // object->material->UI();
            // TODO: Cast to correct object type and display settings
        }
    }
    if (ImGui::CollapsingHeader("Camera Options")) {
        Camera& camera = m_CurrentScene->GetCamera();
        UI::InputFloat3("Camera Position", &camera.position.x);
        UI::InputFloat3("Camera Direction", &camera.direction.x);
        UI::SliderFloat("Camera Speed", &camera.speed, 0.1f, 10.0f);
        UI::InputFloat("Camera Vertical FOV", &camera.vfov);
        UI::Checkbox("Camera Lens", &camera.useLens);
        UI::SliderFloat("Camera Aperture", &camera.aperture, 0.0f, 4.0f);
        UI::SliderFloat("Camera Focus Distance", &camera.focus_dist, 0.0f, 20.0f);
    }
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
    camera->SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
    camera->SetDirection(glm::vec3(0.0f, 0.0f, 1.0f));
    S<Scene> scene = CreateS<Scene>("First Scene", glm::vec3(0.0f), camera);
    scene->SetSkyColor(0.70, 0.80, 1.00);
    scene->Add<Sphere>("Sphere 1", glm::vec3(0, 0, 0), CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f);
    scene->Add<Sphere>("Sphere 2", glm::vec3(2.0, 0, 0), CreateS<Metal>(glm::vec3(0.8f), 0.3f), 1.0f);
    scene->Add<Sphere>("Sphere 3", glm::vec3(-2.0, 0, 0), CreateS<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f), 1.0f);
    // scene->Add<Box>("Box", glm::vec3(5, 0, 0), CreateS<Lambertian>(glm::vec3(0.5f)), glm::vec3(1, 1, 1)));
    scene->Add<Plane>("Plane", glm::vec3(0, -1, 0), CreateS<Lambertian>(glm::vec3(0.8, 0.8, 0.0f)), glm::vec3(0, 1, 0));
    return scene;
}

S<Scene> SceneManager::MaterialTestScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->SetPosition(glm::vec3(2.0f, 4.0f, 5.0f));
    camera->SetDirection(glm::vec3(-2.0f, -4.0f, -5.0f));
    S<Scene> scene = CreateS<Scene>("Material Test", glm::vec3(0.0f), camera);
    scene->SetSkyColor(0.70, 0.80, 1.00);
    scene->Add<Sphere>("Ground", glm::vec3(0, -1000.0f, 0), CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f);
    scene->Add<Box>("Red Cube", glm::vec3(2.0f, 1.0f, -2.0f), CreateS<Lambertian>(glm::vec3(0.5f, 0.0f, 0.0f)), glm::vec3(1.0f));
    scene->Add<Sphere>("Green Sphere", glm::vec3(-2.0f, 1.0f, 2.0f), CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.0f)), 1.0f);
    scene->Add<Sphere>("Metal Sphere", glm::vec3(2.0f, 1.0f, 2.0f), CreateS<Metal>(glm::vec3(0.6f), 0.5f), 1.0f);
    // scene->Add<Sphere>("Purple Sphere", glm::vec3(-2.0f, 1.0f, -2.0f), CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.5f)), 1.0f);
    scene->Add<Rectangle>("Purple Rectangle", glm::vec3(-2.5f, 1.0f, -2.5f), CreateS<Lambertian>(glm::vec3(0.0f, 0.5f, 0.5f)), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scene->Add<Sphere>("Glass Sphere", glm::vec3(0.0f, 1.0f, 0.0f), CreateS<Dielectric>(1.5f), 1.0f);
    return scene;
}

S<Scene> SceneManager::LensTestScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
    camera->SetDirection(glm::vec3(0.0f, 0.0f, 1.0f));
    S<Scene> scene = CreateS<Scene>("Lens Test", glm::vec3(0.0f), camera);
    scene->SetSkyColor(0.70, 0.80, 1.00);
    scene->Add<Sphere>("Sphere 1", glm::vec3(0, 0, 0), CreateS<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f)), 1.0f);
    scene->Add<Sphere>("Ground", glm::vec3(0, -1001.0f, 0), CreateS<Lambertian>(glm::vec3(1.0f)), 1000.0f);
    return scene;
}

S<Scene> SceneManager::RandomLargeScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->SetPosition(glm::vec3(13.0f, 2.0f, 3.0f));
    camera->SetDirection(glm::vec3(-13.0f, -2.0f, -3.0f));

    S<Scene> scene = CreateS<Scene>("Random Large", glm::vec3(0.0f), camera);
    scene->SetSkyColor(0.70, 0.80, 1.00);

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

S<Scene> SceneManager::LightTestScene() {
    S<Camera> camera = CreateS<Camera>();
    camera->SetPosition(glm::vec3(2.0f, 7.0f, 5.0f));
    camera->SetDirection(glm::vec3(-2.0f, -7.0f, -5.0f));

    S<Scene> scene = CreateS<Scene>("Light Test", glm::vec3(0.0f), camera);
    scene->Add<Sphere>("Ground", glm::vec3(0, -1000.0f, 0), CreateS<Lambertian>(glm::vec3(0.5f)), 1000.0f);
    scene->Add<Sphere>("Sphere", glm::vec3(0.0f, 2.0f, 0.0f), CreateS<Lambertian>(glm::vec3(0.5f)), 2.0f);
    // scene->Add<Sphere>("Light", glm::vec3(0.0f, 5.0f, 0.0f), CreateS<DiffuseLight>(glm::vec3(1.0f), 4.0f), 0.5f);
    scene->Add<Rectangle>("Light", glm::vec3(-0.5f, 5.0f, -0.5f), CreateS<DiffuseLight>(glm::vec3(1.0f), 4.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    return scene;
}

S<Scene> SceneManager::CornellBox() {
    S<Camera> camera = CreateS<Camera>();
    camera->SetPosition(glm::vec3(0, 256, -600));
    camera->SetDirection(glm::vec3(0, 0, 1));

    S<Scene> scene = CreateS<Scene>("Cornell Box", glm::vec3(0.0f), camera);

    S<Lambertian> red = CreateS<Lambertian>(glm::vec3(0.65f, 0.05f, 0.05f));
    S<Lambertian> white = CreateS<Lambertian>(glm::vec3(0.73f));
    S<Lambertian> green = CreateS<Lambertian>(glm::vec3(0.12f, 0.45f, 0.15f));
    S<DiffuseLight> light = CreateS<DiffuseLight>(glm::vec3(1.0f), 15.0f);

    scene->Add<Rectangle>("Floor", glm::vec3(-256, 0, -256), white, glm::vec3(0, 0, 512), glm::vec3(512, 0, 0));
    scene->Add<Rectangle>("Ceiling", glm::vec3(-256, 512, -256), white, glm::vec3(0, 0, 512), glm::vec3(512, 0, 0));
    scene->Add<Rectangle>("Back Wall", glm::vec3(-256, 0, 256), white, glm::vec3(0, 512, 0), glm::vec3(512, 0, 0));
    scene->Add<Rectangle>("Left Wall", glm::vec3(256, 0, -256), green, glm::vec3(0, 512, 0), glm::vec3(0, 0, 512));
    scene->Add<Rectangle>("Right Wall", glm::vec3(-256, 0, -256), red, glm::vec3(0, 512, 0), glm::vec3(0, 0, 512));

    S<Box> box1 = CreateS<Box>("Box 1", glm::vec3(80, 150.0f, 80), white, glm::vec3(80.0f, 150.0f, 80.0f));
    S<Box> box2 = CreateS<Box>("Box 2", glm::vec3(-110, 80, -80), white, glm::vec3(80.0f));
    Transform* t1 = scene->Add<Transform>(box1);
    Transform* t2 = scene->Add<Transform>(box2);
    t1->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    t1->rotationAngle = 25.0f;
    t2->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    t2->rotationAngle = -23.0f;

    scene->Add<Rectangle>("Light", glm::vec3(-64, 511, -64), light, glm::vec3(0, 0, 128), glm::vec3(128, 0, 0));

    return scene;
}

}  // namespace raytracing