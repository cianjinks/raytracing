#pragma once

#include "UI.h"
#include "Window.h"

namespace raytracing {

class Application {
   private:
    static Window* s_Window;

   public:
    Application();
    ~Application();

    void Setup();
    void Run();

    static Window* GetWindow() { return s_Window; }
};

}  // namespace raytracing