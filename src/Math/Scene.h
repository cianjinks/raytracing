#pragma once

#include "BVH.h"
#include "Camera.h"
#include "Material.h"
#include "Object.h"

namespace raytracing {

class Scene : public Object {
   private:
    std::vector<S<Hittable>> m_Objects;
    U<BVHNode> m_BVH;
    S<Camera> m_Camera; /* Scenes only support 1 camera. */

    glm::vec3 m_SkyColor = glm::vec3(0.0f);

   public:
    Scene(std::string name, glm::vec3 position, S<Camera> camera);
    ~Scene();

    template <class T, class... Args>
    S<T> Add(Args&&... args) {
        return std::dynamic_pointer_cast<T>(m_Objects.emplace_back(CreateS<T>(std::forward<Args>(args)...)));
    }

    bool Hit(const Ray& ray, uint32_t& seed, float t_min, float t_max, HitResult& hit) const override;

    void BuildBVH();

    inline std::vector<S<Hittable>>& GetObjects() { return m_Objects; }
    inline Camera& GetCamera() const { return *m_Camera.get(); }

    inline const glm::vec3& GetSkyColor() const { return m_SkyColor; }
    inline glm::vec3& GetSkyColor() { return m_SkyColor; }

    inline void SetSkyColor(const glm::vec3& color) { m_SkyColor = color; }
    inline void SetSkyColor(float r, float g, float b) {
        m_SkyColor.r = r;
        m_SkyColor.g = g;
        m_SkyColor.b = b;
    }
};

class SceneManager {
   private:
    std::vector<S<Scene>> m_SceneList;
    S<Scene> m_CurrentScene = nullptr;
    uint32_t m_CurrentSceneIndex = 0;

   public:
    SceneManager();
    ~SceneManager() = default;

    void UI();

    inline Scene& GetScene() const { return *m_CurrentScene.get(); }

   private:
    void SceneComboUI();

    S<Scene> FirstScene();
    S<Scene> MaterialTestScene();
    S<Scene> LensTestScene();
    S<Scene> RandomLargeScene();
    S<Scene> LightTestScene();
    S<Scene> CornellBox();
    S<Scene> BVHTest();
    S<Scene> RectTest();
    S<Scene> TextureTest();
    S<Scene> VolumeCornellBox();
};

}  // namespace raytracing