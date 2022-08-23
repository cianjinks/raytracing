#pragma once

#include <glm/glm.hpp>

namespace raytracing {

/* TODO: Create ImageFormat. */

struct Pixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    Pixel() {}
    Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    Pixel(glm::vec3 c) {
        r = (uint8_t)(c.x * 255.0f);
        g = (uint8_t)(c.y * 255.0f);
        b = (uint8_t)(c.z * 255.0f);
    }
};

class Image {
   private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_PixelCount;
    Pixel* m_Data;

   public:
    Image(uint32_t width, uint32_t height);
    ~Image();

    void Resize(uint32_t width, uint32_t height);

    void Fill(Pixel pixel);
    void Fill(Pixel pixel, float percentage);

    void Randomize(); /* Randomize every pixel. */

    void PerPixel(
        std::function<Pixel(Image* image, uint32_t x, uint32_t y)> func);

    Pixel* GetData() { return m_Data; }
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }
    float GetAspectRatio() { return float(m_Width) / float(m_Height); }
    uint32_t GetPixelCount() { return m_PixelCount; }
};

}  // namespace raytracing