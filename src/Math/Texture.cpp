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

// Mostly taken from:
// https://github.com/stegu/psrdnoise/blob/main/src/psrdnoise2-min.glsl
// m_Scale scales the texture
// m_Period is the desired periods along x and y, and
// m_Alpha is the rotation (in radians) for the swirling gradients.
glm::vec3 PerlinNoiseTexture2D::sample(float u, float v, const glm::vec3& p) const {
    glm::vec2 x = (1.0f / m_Scale) * glm::vec2(u, v);
    glm::vec2 uv = glm::vec2(x.x+x.y*0.5f, x.y);
    glm::vec2 i0 = glm::floor(uv), f0 = glm::fract(uv);
    float cmp = glm::step(f0.y, f0.x);
    glm::vec2 o1 = glm::vec2(cmp, 1.0f-cmp);
    glm::vec2 i1 = i0 + o1, i2 = i0 + 1.0f;
    glm::vec2 v0 = glm::vec2(i0.x - i0.y*0.5f, i0.y);
    glm::vec2 v1 = glm::vec2(v0.x + o1.x - o1.y*0.5f, v0.y + o1.y);
    glm::vec2 v2 = glm::vec2(v0.x + 0.5f, v0.y + 1.0f);
    glm::vec2 x0 = x - v0, x1 = x - v1, x2 = x - v2;
    glm::vec3 iu, iv, xw, yw;
    if(glm::any(glm::greaterThan(m_Period, glm::vec2(0.0f)))) {
        xw = glm::vec3(v0.x, v1.x, v2.x);
        yw = glm::vec3(v0.y, v1.y, v2.y);
        if(m_Period.x > 0.0f)
        xw = glm::mod(glm::vec3(v0.x, v1.x, v2.x), m_Period.x);
        if(m_Period.y > 0.0f)
        yw = glm::mod(glm::vec3(v0.y, v1.y, v2.y), m_Period.y);
        iu = floor(xw + 0.5f*yw + 0.5f); iv = glm::floor(yw + 0.5f);
    } else {
        iu = glm::vec3(i0.x, i1.x, i2.x); iv = glm::vec3(i0.y, i1.y, i2.y);
    }
    glm::vec3 hash = glm::mod(iu, 289.0f);
    hash = glm::mod((hash*51.0f + 2.0f)*hash + iv, 289.0f);
    hash = glm::mod((hash*34.0f + 10.0f)*hash, 289.0f);
    glm::vec3 psi = hash*0.07482f + m_Alpha;
    glm::vec3 gx = glm::cos(psi); glm::vec3 gy = glm::sin(psi);
    glm::vec2 g0 = glm::vec2(gx.x, gy.x);
    glm::vec2 g1 = glm::vec2(gx.y, gy.y);
    glm::vec2 g2 = glm::vec2(gx.z, gy.z);
    glm::vec3 w = 0.8f - glm::vec3(glm::dot(x0, x0), glm::dot(x1, x1), glm::dot(x2, x2));
    w = glm::max(w, 0.0f); glm::vec3 w2 = w*w; glm::vec3 w4 = w2*w2;
    glm::vec3 gdotx = glm::vec3(glm::dot(g0, x0), glm::dot(g1, x1), glm::dot(g2, x2));
    float n = glm::dot(w4, gdotx);
    // glm::vec3 w3 = w2*w; glm::vec3 dw = -8.0f*w3*gdotx;
    // glm::vec2 dn0 = w4.x*g0 + dw.x*x0;
    // glm::vec2 dn1 = w4.y*g1 + dw.y*x1;
    // glm::vec2 dn2 = w4.z*g2 + dw.z*x2;
    // gradient = 10.9f*(dn0 + dn1 + dn2);
    n = ((10.9f * n) + 1.0f) / 2.0f;
    return glm::vec3(n);
}

} // namespace raytracing