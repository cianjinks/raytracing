#pragma once

#include "Camera.h"
#include "Image.h"

/**
 * @brief ImageView
 * The ImageView class represents a 2D image/texture rendered to the
 * screen using OpenGL. It provides functionality for panning, zooming
 * and resizing.
 */

namespace raytracing {

class ImageView {
   private:
    uint32_t m_WindowWidth;
    uint32_t m_WindowHeight;
    uint32_t m_ImageWidth;
    uint32_t m_ImageHeight;
    float m_FWindowWidth;
    float m_FWindowHeight;
    float m_FImageWidth;
    float m_FImageHeight;

    Image* m_Image;
    Camera* m_Camera;

    uint32_t m_ShaderProgramID;
    uint32_t m_CameraUniformID;

   public:
    ImageView(uint32_t window_width, uint32_t window_height,
              uint32_t image_width, uint32_t image_height);
    ~ImageView();

    void OnUpdate();
};

}  // namespace raytracing