#include "Application.h"

int main(void) {
    raytracing::Application *app = new raytracing::Application();
    app->Run();
    delete app;
}