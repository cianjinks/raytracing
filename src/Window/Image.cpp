#include "Image.h"

#include <stb_image_write.h>

namespace raytracing {

Image::Image(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height) {
    m_PixelCount = width * height;
    m_Data = new Pixel[m_PixelCount];
}

Image::~Image() { delete[] m_Data; }

void Image::Save(const char* filepath) {
    stbi_flip_vertically_on_write(true);
    int stride_in_bytes = m_Width * sizeof(Pixel); /* Size of a row in bytes. */
    int result = stbi_write_png(filepath, (int)m_Width, (int)m_Height, 3, m_Data, stride_in_bytes);
    if (result) {
        RT_LOG("Image has been saved to: {}", filepath);
    } else {
        RT_ERROR("Failed to save image to: {}", filepath);
    }
}

void Image::Resize(uint32_t width, uint32_t height) {
    RT_PROFILE_FUNC;
    m_Width = width;
    m_Height = height;
    m_PixelCount = width * height;
    delete[] m_Data;
    m_Data = new Pixel[m_PixelCount];
    Randomize();
}

void Image::Fill(Pixel pixel) {
    RT_PROFILE_FUNC;
    for (uint32_t p = 0; p < m_PixelCount; p++) {
        m_Data[p] = pixel;
    }
}

void Image::Fill(Pixel pixel, float percentage) {
    RT_PROFILE_FUNC;
    uint32_t amount = (uint32_t)(((float)m_PixelCount) * percentage);
    for (uint32_t p = 0; p < amount; p++) {
        m_Data[p] = pixel;
    }
}

void Image::Randomize() {
    RT_PROFILE_FUNC;
    uint8_t v = std::rand() % 256;
    for (uint32_t p = 0; p < m_PixelCount; p++) {
        m_Data[p].r = v;
        m_Data[p].g = v;
        m_Data[p].b = v;
        v = std::rand() % 256;
    }
}

void Image::PerPixel(
    std::function<Color(Image* image, uint32_t x, uint32_t y)> func) {
    RT_PROFILE_FUNC;
    for (uint32_t w = 0; w < m_Width; w++) {
        for (uint32_t h = 0; h < m_Height; h++) {
            uint64_t index = (m_Width * h) + w;
            Color color = func(this, w, h);
            m_Data[index] = Pixel(color);
        }
    }
}

void Image::PerSample(std::function<Color(Image* image, uint32_t x, uint32_t y, uint32_t s)> func,
                      uint32_t max_samples) {
    RT_PROFILE_FUNC;
    for (uint32_t w = 0; w < m_Width; w++) {
        for (uint32_t h = 0; h < m_Height; h++) {
            SetPixelSampled(func, max_samples, w, h);
        }
    }
}

void Image::PerSampleSection(std::function<Color(Image* image, uint32_t x, uint32_t y, uint32_t s)> func,
                             uint32_t max_samples, uint32_t sx, uint32_t sy, uint32_t swidth,
                             uint32_t sheight) {
    RT_PROFILE_FUNC;
    for (uint32_t w = sx; w < sx + swidth; w++) {
        for (uint32_t h = sy; h < sy + sheight; h++) {
            SetPixelSampled(func, max_samples, w, h);
        }
    }
}

void Image::SetPixelSampled(std::function<Color(Image* image, uint32_t x, uint32_t y, uint32_t s)> func,
                            uint32_t max_samples, uint32_t w, uint32_t h) {
    RT_PROFILE_FUNC;
    Color color = {0.0f, 0.0f, 0.0f};
    for (uint32_t s = 0; s < max_samples; s++) {
        color += func(this, w, h, s);
    }
    color = glm::sqrt(color * (1.0f / max_samples));

    uint64_t index = (m_Width * h) + w;
    m_Data[index] = Pixel(color);
}
}  // namespace raytracing