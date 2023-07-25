#include "Texture2D.h"

namespace raytracing {

template <>
void Texture2D<uint8_t, 3>::Randomize() {
    uint8_t v = std::rand() % 256;
    for (uint32_t e = 0; e < m_ElementCount; e += 3) {
        m_Data[e + 0] = v;
        m_Data[e + 1] = v;
        m_Data[e + 2] = v;
        v = std::rand() % 256;
    }
}

template <>
void Texture2D<uint8_t, 3>::Save(const char* filepath) {
    stbi_flip_vertically_on_write(true);
    int stride_in_bytes = m_Width * sizeof(uint8_t) * 3; /* Size of a row in bytes. */
    int result = stbi_write_png(filepath, (int)m_Width, (int)m_Height, 3, m_Data, stride_in_bytes);
    if (result) {
        RT_LOG("Image has been saved to: {}", filepath);
    } else {
        RT_ERROR("Failed to save image to: {}", filepath);
    }
}

}