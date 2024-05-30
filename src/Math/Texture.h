#pragma once

#include <glm/glm.hpp>

#include "Render/Image.h"

namespace raytracing {

class Texture {
   public:
    virtual ~Texture() = default;

    virtual glm::vec3 sample(float u, float v, const glm::vec3& p) const = 0;
};

class SingleColorTexture : public Texture {
   public:
    SingleColorTexture(const glm::vec3& albedo) : m_Albedo(albedo) {}

    glm::vec3 sample(float u, float v, const glm::vec3& p) const override;

   private:
    glm::vec3 m_Albedo;
};

class CheckerTexture : public Texture {
   public:
    CheckerTexture(float scale, S<Texture> c1, S<Texture> c2)
        : m_InvScale(1.0f / scale), m_C1(c1), m_C2(c2) {}
    CheckerTexture(float scale, const glm::vec3& c1, const glm::vec3& c2)
        : m_InvScale(1.0f / scale), m_C1(CreateS<SingleColorTexture>(c1)), m_C2(CreateS<SingleColorTexture>(c2)) {}

    glm::vec3 sample(float u, float v, const glm::vec3& p) const override;

   private:
    float m_InvScale = 1.0f;
    S<Texture> m_C1 = nullptr;
    S<Texture> m_C2 = nullptr;
};

class ImageTexture : public Texture {
   public:
    // TODO: If the image class was run-time polymorphic we could support arbitrary image formats
    //       Or maybe it's better to force every image to be Image2Du8 instead of 3f?
    ImageTexture(S<Image2D3f> image) : m_Image(image) {}

    glm::vec3 sample(float u, float v, const glm::vec3& p) const override;

   private:
    S<Image2D3f> m_Image = nullptr;
};

class PerlinNoiseTexture2D : public Texture {
   public:
    PerlinNoiseTexture2D(float scale, const glm::vec2& period, float alpha) : m_Scale(scale), m_Period(period), m_Alpha(alpha) {}

    glm::vec3 sample(float u, float v, const glm::vec3& p) const override;

   private:
    float m_Scale = 1.0f;
    glm::vec2 m_Period;
    float m_Alpha;
};

} // namespace raytracing