#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

#include "moci/render/opengl/gl4/gl4.hpp"

namespace moci
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data);

    ~OpenGLTexture2D() override;

    [[nodiscard]] uint32_t GetWidth() const override { return m_Width; }
    [[nodiscard]] uint32_t GetHeight() const override { return m_Height; }
    [[nodiscard]] uint32_t GetID() const override { return m_RendererID; }

    void SetData(void* data, uint32_t size);

    void Bind(uint32_t slot = 0) const override;
    void Unbind() const override { }

private:
    std::string m_Path;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
    GLenum m_InternalFormat, m_DataFormat;
};

class OpenGLTextureCube : public TextureCube
{
public:
    OpenGLTextureCube(Vector<std::string> paths);

    OpenGLTextureCube(const OpenGLTextureCube&) = delete;
    OpenGLTextureCube& operator=(const OpenGLTextureCube&) = delete;

    ~OpenGLTextureCube() override;

    [[nodiscard]] std::uint32_t GetWidth() const override { return 0; }
    [[nodiscard]] std::uint32_t GetHeight() const override { return 0; }
    [[nodiscard]] std::uint32_t GetID() const override { return renderID_; }

    void Bind(std::uint32_t slot = 0) const override;
    void Unbind() const override;

private:
    Vector<std::string> paths_;
    std::uint32_t renderID_ {};
};

}  // namespace moci

#endif