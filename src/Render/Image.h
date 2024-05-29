#pragma once

#include <stb_image.h>
#include <stb_image_write.h>

#include <glm/glm.hpp>

namespace raytracing {

class Image2DHelper {
   public:
    // Cast pixels from one type to another, necessary to save and load images
    template<typename FromType, typename ToType>
    static ToType CastPixel(FromType from) {
        /* Cast in the general case. Also see specializations below. */
        return (ToType)from;
    }

    // Generate a random pixel value for a given type
    template<typename T>
    static T RandomPixel(uint32_t& seed) {
        static_assert(false, "Image2DHelper::RandomPixel is not implemented in the general case.");
        return T(0);
    }
};

template<>
inline float Image2DHelper::CastPixel(uint8_t from) {
    return (float)from / 255.0f;
}

template<>
inline uint8_t Image2DHelper::CastPixel(float from) {
    return (uint8_t)(from * 255.0f);
}

template<>
inline float Image2DHelper::RandomPixel(uint32_t& seed) {
    return FastRandom::Float(seed);
}

template<>
inline uint8_t Image2DHelper::RandomPixel(uint32_t& seed) {
    return FastRandom::U8(seed);
}

template <typename T, unsigned int N>
class Image2D {
   private:
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    uint32_t m_PixelCount = 0;    // TODO: Overflow for large m_Width and m_Height
    uint32_t m_ElementCount = 0;  // TODO: Overflow for large m_PixelCount

    T* m_Data = nullptr;

   private:
    void Invalidate() {
        if (IsValid()) {
            delete[] m_Data;
            m_Width = 0;
            m_Height = 0;
            m_PixelCount = 0;
            m_ElementCount = 0;
            m_Data = nullptr;
        }
    }

   public:
    Image2D(uint32_t width, uint32_t height) {
        Resize(width, height);
    };

    Image2D(const char* filepath) {
        Load(filepath);
    }

    ~Image2D() { Invalidate(); }

    T* GetData() { return m_Data; }
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }
    float GetAspectRatio() { return float(m_Width) / float(m_Height); }
    uint32_t GetPixelCount() { return m_PixelCount; }
    uint32_t GetElementCount() { return m_ElementCount; }

    bool IsValid() {
        return m_Data != nullptr;
    }

    void Resize(uint32_t width, uint32_t height) {
        RT_PROFILE_FUNC;

        Invalidate();

        m_Width = width;
        m_Height = height;
        m_PixelCount = width * height;
        m_ElementCount = m_PixelCount * N;
        m_Data = new T[m_ElementCount];

        Clear();
    }

    void Clear() {
        RT_PROFILE_FUNC;

        std::fill_n(m_Data, m_ElementCount, T(0));
    }

    void Fill(const glm::vec<N, T>& val) {
        RT_PROFILE_FUNC;

        for (uint32_t i = 0; i < m_ElementCount; i += N) {
            for (uint32_t n = 0; n < N; n++) {
                m_Data[i + n] = val[n];
            }
        }
    }

    void Randomize(uint32_t&& seed) {
        for (uint32_t e = 0; e < m_ElementCount; e += N) {
            for (uint32_t n = 0; n < N; n++) {
                m_Data[e + n] = Image2DHelper::RandomPixel<T>(seed);
            }
        }
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
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0, num_channels = 0;
        stbi_uc* data = stbi_load(filepath, &width, &height, &num_channels, 3);
        if (data) {
            RT_LOG("Loaded image: {}, width = {}, height = {}, num_channels = {}", filepath, width, height, num_channels);
            // Resize
            Resize(width, height);
            // Fill in data
            for (int32_t y = 0; y < height; y++) {
                for (int32_t x = 0; x < width; x++) {
                    int32_t index = ((y * width) + x) * N;
                    for (uint32_t n = 0; n < N; n++) {
                        m_Data[index + n] = Image2DHelper::CastPixel<uint8_t, float>(data[index + n]);
                    }
                }
            }
            // Free
            stbi_image_free(data);
        } else {
            RT_ERROR("Failed to load image: {}", filepath);
        }
    }

    void Save(const char* filepath) {
        stbi_flip_vertically_on_write(true);

        // Copy image data to temporary uint8_t format buffer
        // TODO: It would be nice if we didn't have to do this. Maybe can use `stbi_write_png_to_func`?
        uint8_t* temp_image_data = new uint8_t[m_ElementCount];
        for (uint32_t e = 0; e < m_ElementCount; e += N) {
            for (uint32_t n = 0; n < N; n++) {
                temp_image_data[e + n] = Image2DHelper::CastPixel<T, uint8_t>(m_Data[e + n]);
            }
        }

        // Save Image
        int stride_in_bytes = m_Width * sizeof(uint8_t) * N; /* Size of a row in bytes. */
        int result = stbi_write_png(filepath, (int)m_Width, (int)m_Height, N, temp_image_data, stride_in_bytes);
        if (result) {
            RT_LOG("Image has been saved to: {}", filepath);
        } else {
            RT_ERROR("Failed to save image to: {}", filepath);
        }

        // Delete temporary buffer
        delete[] temp_image_data;
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

}  // namespace raytracing