#pragma once

#include "moci/render/texture.hpp"

namespace moci
{

class OpenGLESTexture2D : public Texture2D
{
public:
    explicit OpenGLESTexture2D(std::string path);
    OpenGLESTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data);
    ~OpenGLESTexture2D() override;

    OpenGLESTexture2D(const OpenGLESTexture2D&)                    = delete;
    auto operator=(const OpenGLESTexture2D&) -> OpenGLESTexture2D& = delete;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return width_; }
    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return height_; }
    [[nodiscard]] auto getId() const -> std::uint32_t override { return renderID_; }

    void bind(std::uint32_t slot = 0) const override;
    void unbind() const override;

private:
    void createTexture();
    static void setFilterModes();

    std::uint32_t renderID_ = {};
    std::int32_t width_     = {};
    std::int32_t height_    = {};
    std::int32_t bpp_       = {};
    std::uint8_t* data_     = nullptr;
    std::string path_       = {};
};

class OpenGLESTextureCube : public TextureCube
{
public:
    explicit OpenGLESTextureCube(Vector<std::string> paths);

    OpenGLESTextureCube(const OpenGLESTextureCube&)                    = delete;
    auto operator=(const OpenGLESTextureCube&) -> OpenGLESTextureCube& = delete;

    ~OpenGLESTextureCube() override;

    [[nodiscard]] auto getWidth() const -> std::uint32_t override { return 0; }
    [[nodiscard]] auto getHeight() const -> std::uint32_t override { return 0; }
    [[nodiscard]] auto getId() const -> std::uint32_t override { return renderID_; }

    void bind(std::uint32_t slot = 0) const override;
    void unbind() const override;

private:
    Vector<std::string> paths_;
    std::uint32_t renderID_ {};
};
}  // namespace moci