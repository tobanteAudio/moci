#pragma once

#include "moci_render_base/moci_render_base.hpp"

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

    [[nodiscard]] auto GetWidth() const -> std::uint32_t override { return width_; }
    [[nodiscard]] auto GetHeight() const -> std::uint32_t override { return height_; }
    [[nodiscard]] auto GetID() const -> std::uint32_t override { return renderID_; }

    void Bind(std::uint32_t slot = 0) const override;
    void Unbind() const override;

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

    [[nodiscard]] auto GetWidth() const -> std::uint32_t override { return 0; }
    [[nodiscard]] auto GetHeight() const -> std::uint32_t override { return 0; }
    [[nodiscard]] auto GetID() const -> std::uint32_t override { return renderID_; }

    void Bind(std::uint32_t slot = 0) const override;
    void Unbind() const override;

private:
    Vector<std::string> paths_;
    std::uint32_t renderID_ {};
};
}  // namespace moci