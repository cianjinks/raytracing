#pragma once

#include "Material.h"
#include "Object.h"

namespace raytracing {

class Scene : public Object {
   private:
    std::vector<U<Object>> m_Objects;
    S<Camera> m_Camera; /* Scenes only support 1 camera. */

   public:
    Scene(std::string name, glm::vec3 position, S<Camera> camera);
    ~Scene();

    template <class T, class... Args>
    void Add(Args&&... args) {
        m_Objects.emplace_back(CreateU<T>(std::forward<Args>(args)...));
    }

    bool Hit(const Ray& ray, float t_min, float t_max, HitResult& hit) const override;

    std::vector<U<Object>>& GetObjects() { return m_Objects; }
    inline Camera& GetCamera() const { return *m_Camera.get(); }
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
};

}  // namespace raytracing