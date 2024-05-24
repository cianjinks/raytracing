#include "Texture.h"

namespace raytracing {

glm::vec3 SingleColorTexture::sample(float u, float v, const glm::vec3& p) const  {
    return m_Albedo;
}

glm::vec3 CheckerTexture::sample(float u, float v, const glm::vec3& p) const {
    glm::i32vec3 ip = glm::i32vec3(glm::floor(m_InvScale * p));
    bool is_even = (ip.x + ip.y + ip.z) % 2 == 0;
    return is_even ? m_C1->sample(u, v, p) : m_C2->sample(u, v, p);
}

glm::vec3 ImageTexture::sample(float u, float v, const glm::vec3& p) const {
    uint32_t i = (uint32_t)(u * (float)(m_Image->GetWidth() - 1));
    uint32_t j = (uint32_t)(v * (float)(m_Image->GetHeight() - 1));
    if (i < m_Image->GetWidth() && j < m_Image->GetHeight()) {
        return m_Image->at(i, j);
    }
    return glm::vec3(0.4f, 0.0f, 0.235f);
}

} // namespace raytracing