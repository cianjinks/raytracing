#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> /* TODO: Create own keymaps. */

#include <glm/glm.hpp>

#include "Application.h"

/**
 * @brief Input
 * As opposed to Event, Input is for polling input.
 */

namespace raytracing {

class Input {
   public:
    static bool IsKeyPressed(int key);
    static bool IsMouseButtonPressed(int button);
    static glm::dvec2 GetMouseCoord();
    static double GetMouseX();
    static double GetMouseY();
};

}  // namespace raytracing