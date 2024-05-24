#pragma once

#include <stb_image.h>
#include <stb_image_write.h>

#include <glm/glm.hpp>

namespace raytracing {

template <typename T, unsigned int N>
class Image2D {
   private:
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    uint32_t m_PixelCount = 0;    // TODO: Overflow for large m_Width and m_Height
    uint32_t m_ElementCount = 0;  // TODO: Overflow for large m_PixelCount

    T* m_Data = nullptr;

   public:
    Image2D(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) {
        m_PixelCount = width * height;
        m_ElementCount = m_PixelCount * N;
        m_Data = new T[m_ElementCount];
        Clear();
    };

    Image2D(const char* filepath) {
        this->Load(filepath);
    }

    ~Image2D() { delete[] m_Data; }

    T* GetData() { return m_Data; }
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }
    float GetAspectRatio() { return float(m_Width) / float(m_Height); }
    uint32_t GetPixelCount() { return m_PixelCount; }
    uint32_t GetElementCount() { return m_ElementCount; }

    void Resize(uint32_t width, uint32_t height) {
        RT_PROFILE_FUNC;

        m_Width = width;
        m_Height = height;
        m_PixelCount = width * height;
        m_ElementCount = m_PixelCount * N;

        delete[] m_Data;
        m_Data = new T[m_ElementCount];

        Clear();
    }

    void Clear() {
        RT_PROFILE_FUNC;

        std::fill_n(m_Data, m_ElementCount, T(0));
    }

    void Fill(glm::vec<N, T> val) {
        RT_PROFILE_FUNC;

        for (uint32_t i = 0; i < m_ElementCount; i += N) {
            for (uint32_t n = 0; n < N; n++) {
                m_Data[i + n] = val[n];
            }
        }
    }

    void Randomize() {
        /* Do nothing in the general case for now. See specializations below. */
        RT_ERROR("Image2D::Randomize does not work for this image type");
    }

    void PerPixel(
        std::function<glm::vec<N, T>(Image2D* texture, uint32_t x, uint32_t y)> func) {
        RT_PROFILE_FUNC;

        for (uint32_t w = 0; w < m_Width; w++) {
            for (uint32_t h = 0; h < m_Height; h++) {
                glm::vec<N, T> val = func(this, w, h);
                at(w, h) = val;
            }
        }
    }

    void Load(const char* filepath) {
        /* Do nothing in the general case for now. See specializations below. */
        RT_ERROR("Image2D::Load does not work for this image type");
    }

    void Save(const char* filepath) {
        /* Do nothing in the general case for now. See specializations below. */
        RT_ERROR("Image2D::Save does not work for this image type");
    }

    glm::vec<N, T>& at(uint32_t w, uint32_t h) {
        // TODO: Bounds check? Can't because we return a reference.
        uint64_t index = ((m_Width * h) + w) * N;
        T* ptr = &m_Data[index];
        return *(glm::vec<N, T>*)ptr;  // Bad, but no other way
    }

    glm::vec<N, T>& operator()(uint32_t w, uint32_t h) {
        return at(w, h);
    }
};

// ALIASES
typedef Image2D<float, 3> Image2D3f;
typedef Image2D<uint8_t, 3> Image2D3u8;
//

template <>
inline void Image2D3f::Load(const char* filepath) {
    stbi_set_flip_vertically_on_load(true);
    int32_t width = 0, height = 0, num_channels = 0;
    stbi_uc* data = stbi_load(filepath, &width, &height, &num_channels, 3);
    if (data) {
        RT_LOG("Loaded image: {}, width = {}, height = {}, num_channels = {}", filepath, width, height, num_channels);
        // Resize
        this->Resize(width, height);
        // Fill in data
        for (int32_t y = 0; y < height; y++) {
            for (int32_t x = 0; x < width; x++) {
                int32_t index = ((y * width) + x) * 3;
                m_Data[index + 0] = (float)data[index + 0] / 255.0f;
                m_Data[index + 1] = (float)data[index + 1] / 255.0f;
                m_Data[index + 2] = (float)data[index + 2] / 255.0f;
            }
        }
        // Free
        stbi_image_free(data);
    } else {
        RT_ERROR("Failed to load image: {}", filepath);
    }
}

template <>
inline void Image2D3u8::Randomize() {
    uint8_t v = std::rand() % 256;
    for (uint32_t e = 0; e < m_ElementCount; e += 3) {
        m_Data[e + 0] = v;
        m_Data[e + 1] = v;
        m_Data[e + 2] = v;
        v = std::rand() % 256;
    }
}

template <>
inline void Image2D3u8::Save(const char* filepath) {
    stbi_flip_vertically_on_write(true);
    int stride_in_bytes = m_Width * sizeof(uint8_t) * 3; /* Size of a row in bytes. */
    int result = stbi_write_png(filepath, (int)m_Width, (int)m_Height, 3, m_Data, stride_in_bytes);
    if (result) {
        RT_LOG("Image has been saved to: {}", filepath);
    } else {
        RT_ERROR("Failed to save image to: {}", filepath);
    }
}

}  // namespace raytracing