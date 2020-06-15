#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_render_base/moci_render_base.hpp"

#include "gl4.hpp"

namespace moci
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(std::uint32_t width, std::uint32_t height);
    OpenGLTexture2D(std::string path);
    OpenGLTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data);

    ~OpenGLTexture2D() override;

    [[nodiscard]] std::uint32_t GetWidth() const override { return width_; }
    [[nodiscard]] std::uint32_t GetHeight() const override { return height_; }
    [[nodiscard]] std::uint32_t GetID() const override { return renderID_; }

    void SetData(Span<std::uint8_t> data);

    void Bind(std::uint32_t slot = 0) const override;
    void Unbind() const override { }

private:
    void createTexture();
    void setFilters();

    std::string path_;
    std::uint32_t width_;
    std::uint32_t height_;
    std::uint32_t channels_;
    std::uint32_t renderID_;
    GLenum internalFormat_;
    GLenum dataFormat_;
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