#pragma once

#include <glm/glm.hpp>

namespace raytracing {

/* TODO: Create ImageFormat. */

using Color = glm::vec3; /* TODO: Temporary wrapper for code clarity. */

struct Pixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    Pixel() {}
    Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    Pixel(glm::vec3 c) {
        r = (uint8_t)(glm::clamp(c.x, 0.0f, 0.999f) * 256.0f);
        g = (uint8_t)(glm::clamp(c.y, 0.0f, 0.999f) * 256.0f);
        b = (uint8_t)(glm::clamp(c.z, 0.0f, 0.999f) * 256.0f);
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
        std::function<Color(Image* image, uint32_t x, uint32_t y)> func);

    void PerSample(
        std::function<Color(Image* image, uint32_t x, uint32_t y, uint32_t s)>
            func,
        uint32_t max_samples);
    void PerSampleSection(
        std::function<Color(Image* image, uint32_t x, uint32_t y, uint32_t s)>
            func,
        uint32_t max_samples, uint32_t sx, uint32_t sy, uint32_t swidth,
        uint32_t sheight);

    Pixel* GetData() { return m_Data; }
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }
    float GetAspectRatio() { return float(m_Width) / float(m_Height); }
    uint32_t GetPixelCount() { return m_PixelCount; }
};

}  // namespace raytracing