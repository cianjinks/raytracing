#include "Application.h"

int main(void) {
    raytracing::ApplicationInfo info;
    info.Name = "raytracing";
    info.WindowWidth = 1280;
    info.WindowHeight = 720;
    info.ImageWidth = 320 * 2;
    info.ImageHeight = 180 * 2;

    raytracing::Application *app = new raytracing::Application(info);
    app->Run();
    delete app;
}