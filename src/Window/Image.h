#pragma once

namespace raytracing {

/* TODO: Create ImageFormat. */

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
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

    void Fill(Pixel pixel);
    void Fill(Pixel pixel, float percentage);

    void Randomize(); /* Randomize every pixel. */

    void PerPixel(std::function<Pixel(uint32_t x, uint32_t y)> func);

    Pixel* GetData() { return m_Data; }
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }
    uint32_t GetPixelCount() { return m_PixelCount; }
};

}  // namespace raytracing