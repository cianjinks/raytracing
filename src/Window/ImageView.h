#pragma once

#include "Event.h"
#include "Image.h"
#include "ImageCamera.h"

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
    ImageCamera* m_Camera;

    uint32_t m_ShaderProgramID;
    uint32_t m_CameraUniformID;

   public:
    ImageView(uint32_t window_width, uint32_t window_height,
              uint32_t image_width, uint32_t image_height);
    ~ImageView();

    void OnUpdate();
    void OnEvent(Event& event);
    void UI();

    void ResizeWindow(uint32_t width, uint32_t height);
    void ResizeImage(uint32_t width, uint32_t height);

    Image* GetImage() const { return m_Image; }
};

}  // namespace raytracing