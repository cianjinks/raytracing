#pragma once

#include "Window/UI.h"
#include "Window/Window.h"

namespace raytracing {

struct ApplicationInfo {
    std::string Name;
    uint32_t WindowWidth;
    uint32_t WindowHeight;
    uint32_t ImageWidth;
    uint32_t ImageHeight;
};

class Application {
   public:
    ApplicationInfo AppInfo;

   private:
    static Window* s_Window;
    static ImageView* s_ImageView;

   public:
    Application(ApplicationInfo app_info);
    ~Application();

    void Setup();
    void Run();

    static Window* GetWindow() { return s_Window; }
    static ImageView* GetImageView() { return s_ImageView; }
};

}  // namespace raytracing