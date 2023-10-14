#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"

#include <moci/render/texture.hpp>

#include <span>
namespace moci
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(std::uint32_t width, std::uint32_t height);
    explicit OpenGLTexture2D(std::string path);
    OpenGLTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data);

    ~OpenGLTexture2D() override;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return width_; }
    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return height_; }
    [[nodiscard]] auto getId() const -> std::uint32_t override { return renderID_; }

    void setData(std::span<std::uint8_t> data) const;

    void bind(std::uint32_t slot = 0) const override;
    void unbind() const override { }

private:
    void createTexture();
    void setFilters() const;

    std::string path_;
    std::uint32_t width_;
    std::uint32_t height_;
    std::uint32_t channels_ {};
    std::uint32_t renderID_ {};
    GLenum internalFormat_ {};
    GLenum dataFormat_ {};
};

class OpenGLTextureCube : public TextureCube
{
public:
    explicit OpenGLTextureCube(std::vector<std::string> paths);

    OpenGLTextureCube(const OpenGLTextureCube&)                    = delete;
    auto operator=(const OpenGLTextureCube&) -> OpenGLTextureCube& = delete;

    ~OpenGLTextureCube() override;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return 0; }
    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return 0; }
    [[nodiscard]] auto getId() const -> std::uint32_t override { return renderID_; }

    void bind(std::uint32_t slot = 0) const override;
    void unbind() const override;

private:
    std::vector<std::string> paths_;
    std::uint32_t renderID_ {};
};

}  // namespace moci

#endif
