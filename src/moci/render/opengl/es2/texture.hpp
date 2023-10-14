#pragma once

#include <moci/render/texture.hpp>

namespace moci {

class OpenGLESTexture2D : public Texture2D
{
public:
    explicit OpenGLESTexture2D(std::string path);
    OpenGLESTexture2D(
        Texture::Format format,
        std::uint32_t width,
        std::uint32_t height,
        void* data
    );
    ~OpenGLESTexture2D() override;

    OpenGLESTexture2D(OpenGLESTexture2D const&)                    = delete;
    auto operator=(OpenGLESTexture2D const&) -> OpenGLESTexture2D& = delete;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return _width; }

    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return _height; }

    [[nodiscard]] auto getId() const -> std::uint32_t override { return _renderID; }

    void bind(std::uint32_t slot = 0) const override;
    void unbind() const override;

private:
    void createTexture();
    static void setFilterModes();

    std::uint32_t _renderID = {};
    std::int32_t _width     = {};
    std::int32_t _height    = {};
    std::int32_t _bpp       = {};
    std::uint8_t* _data     = nullptr;
    std::string _path       = {};
};

class OpenGLESTextureCube : public TextureCube
{
public:
    explicit OpenGLESTextureCube(std::vector<std::string> paths);

    OpenGLESTextureCube(OpenGLESTextureCube const&)                    = delete;
    auto operator=(OpenGLESTextureCube const&) -> OpenGLESTextureCube& = delete;

    ~OpenGLESTextureCube() override;

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
