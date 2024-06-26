#include "App/Application.h"

int main(void) {
    raytracing::ApplicationInfo info;
    info.Name = "raytracing";
    info.WindowWidth = 1920;
    info.WindowHeight = 1080;
    info.ImageWidth = 320 * 2;
    info.ImageHeight = 180 * 2;

    raytracing::Application *app = new raytracing::Application(info);
    app->Run();
    delete app;
}