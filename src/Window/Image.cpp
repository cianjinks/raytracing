#include "Image.h"

namespace raytracing {

Image::Image(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height) {
    m_PixelCount = width * height;
    m_Data = new uint8_t[m_PixelCount * 3];
}

Image::~Image() { delete[] m_Data; }

void Image::Fill(uint8_t value) {
    std::fill_n(m_Data, m_PixelCount * 3, value);
}

void Image::Fill(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t p = 0; p < (m_PixelCount * 3); p += 3) {
        m_Data[p + 0] = r;
        m_Data[p + 1] = g;
        m_Data[p + 2] = b;
    }
}

void Image::Fill(uint8_t value, float percentage) {
    uint32_t amount = (uint32_t)(((float)(m_PixelCount * 3)) * percentage);
    std::fill_n(m_Data, amount, value);
}

void Image::Fill(uint8_t r, uint8_t g, uint8_t b, float percentage) {
    uint32_t amount = (uint32_t)(((float)(m_PixelCount * 3)) * percentage);
    for (uint32_t p = 0; p < amount; p += 3) {
        m_Data[p + 0] = r;
        m_Data[p + 1] = g;
        m_Data[p + 2] = b;
    }
}

void Image::Randomize() {
    uint8_t v = std::rand() % 256;
    for (uint32_t p = 0; p < (m_PixelCount * 3); p += 3) {
        m_Data[p + 0] = v;
        m_Data[p + 1] = v;
        m_Data[p + 2] = v;
        v = std::rand() % 256;
    }
}

}  // namespace raytracing