#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

    #include "gl4.hpp"

    #include <moci/render/texture.hpp>

    #include <span>

namespace moci {

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(std::uint32_t width, std::uint32_t height);
    explicit OpenGLTexture2D(std::string path);
    OpenGLTexture2D(
        Texture::Format format,
        std::uint32_t width,
        std::uint32_t height,
        void* data
    );

    ~OpenGLTexture2D() override;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return _width; }

    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return _height; }

    [[nodiscard]] auto getId() const -> std::uint32_t override { return _renderID; }

    void setData(std::span<std::uint8_t> data) const;

    void bind(std::uint32_t slot = 0) const override;

    void unbind() const override {}

private:
    void createTexture();
    void setFilters() const;

    std::string _path;
    std::uint32_t _width;
    std::uint32_t _height;
    std::uint32_t _channels{};
    std::uint32_t _renderID{};
    GLenum _internalFormat{};
    GLenum _dataFormat{};
};

class OpenGLTextureCube : public TextureCube
{
public:
    explicit OpenGLTextureCube(std::vector<std::string> paths);

    OpenGLTextureCube(OpenGLTextureCube const&)                    = delete;
    auto operator=(OpenGLTextureCube const&) -> OpenGLTextureCube& = delete;

    ~OpenGLTextureCube() override;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return 0; }

    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return 0; }

    [[nodiscard]] auto getId() const -> std::uint32_t override { return _renderID; }

    void bind(std::uint32_t slot = 0) const override;
    void unbind() const override;

private:
    std::vector<std::string> _paths;
    std::uint32_t _renderID{};
};

}  // namespace moci

#endif
