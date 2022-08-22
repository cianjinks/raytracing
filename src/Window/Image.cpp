#include "Image.h"

namespace raytracing {

Image::Image(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height) {
    m_PixelCount = width * height;
    m_Data = new Pixel[m_PixelCount];
}

Image::~Image() { delete[] m_Data; }

void Image::Fill(Pixel pixel) {
    for (uint32_t p = 0; p < m_PixelCount; p++) {
        m_Data[p] = pixel;
    }
}

void Image::Fill(Pixel pixel, float percentage) {
    uint32_t amount = (uint32_t)(((float)m_PixelCount) * percentage);
    for (uint32_t p = 0; p < amount; p++) {
        m_Data[p] = pixel;
    }
}

void Image::Randomize() {
    uint8_t v = std::rand() % 256;
    for (uint32_t p = 0; p < m_PixelCount; p++) {
        m_Data[p].r = v;
        m_Data[p].g = v;
        m_Data[p].b = v;
        v = std::rand() % 256;
    }
}

void Image::PerPixel(
    std::function<Pixel(Image* image, uint32_t x, uint32_t y)> func) {
    for (uint32_t w = 0; w < m_Width; w++) {
        for (uint32_t h = 0; h < m_Height; h++) {
            uint64_t index = (m_Width * h) + w;
            Pixel result = func(this, w, h);
            m_Data[index] = result;
        }
    }
}

}  // namespace raytracing