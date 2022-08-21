#pragma once

namespace raytracing {

/* TODO: Create ImageFormat. */

class Image {
   private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_PixelCount;
    uint8_t* m_Data;

   public:
    Image(uint32_t width, uint32_t height);
    ~Image();

    void Fill(uint8_t value); /* Fill r, g, b channels with value. */
    void Fill(uint8_t r, uint8_t g, uint8_t b);
    void Fill(uint8_t value, float percentage);
    void Fill(uint8_t r, uint8_t g, uint8_t b, float percentage);

    void Randomize(); /* Randomize every pixel. */

    uint8_t* GetData() { return m_Data; }
};

}  // namespace raytracing