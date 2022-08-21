#pragma once

namespace raytracing {

class UI {
   public:
    static void Init();
    static void PreRender();
    static void Render();

    static bool WantInput();
};
}  // namespace raytracing