#pragma once

#include "Material.h"
#include "Object.h"

namespace raytracing {

class SceneManager {
   private:
    std::vector<Scene*> m_SceneList;
    Scene* m_CurrentScene = nullptr;
    uint32_t m_CurrentSceneIndex = 0;

   public:
    SceneManager();
    ~SceneManager() = default;

    void UI();

    Scene* GetScene() const { return m_CurrentScene; }

   private:
    void SceneComboUI();
};

}  // namespace raytracing