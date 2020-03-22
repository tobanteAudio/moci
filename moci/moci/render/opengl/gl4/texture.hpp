#pragma once

#include "moci/render/texture.hpp"

#include "moci/render/opengl/gl4/gl4.hpp"

namespace moci
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data) {}

    ~OpenGLTexture2D() override;

    [[nodiscard]] uint32_t GetWidth() const override { return m_Width; }
    [[nodiscard]] uint32_t GetHeight() const override { return m_Height; }
    [[nodiscard]] uint32_t GetID() const override { return m_RendererID; }

    void SetData(void* data, uint32_t size);

    void Bind(uint32_t slot = 0) const override;
    void Unbind() const override {}

private:
    std::string m_Path;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
    GLenum m_InternalFormat, m_DataFormat;
};

}  // namespace moci