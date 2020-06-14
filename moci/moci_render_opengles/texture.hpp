#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{

class OpenGLESTexture2D : public Texture2D
{
public:
    OpenGLESTexture2D(std::string path);
    OpenGLESTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data);
    ~OpenGLESTexture2D() override;

    // Delete the copy constructor/assignment.
    OpenGLESTexture2D(const OpenGLESTexture2D&) = delete;
    auto operator=(const OpenGLESTexture2D&) -> OpenGLESTexture2D& = delete;

    [[nodiscard]] auto GetWidth() const -> std::uint32_t override { return width_; }
    [[nodiscard]] auto GetHeight() const -> std::uint32_t override { return height_; }
    [[nodiscard]] auto GetID() const -> std::uint32_t override { return renderID_; }

    void Bind(std::uint32_t slot = 0) const override;
    void Unbind() const override;

private:
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
    OpenGLESTextureCube(Vector<std::string> paths);

    OpenGLESTextureCube(const OpenGLESTextureCube&) = delete;
    OpenGLESTextureCube& operator=(const OpenGLESTextureCube&) = delete;

    ~OpenGLESTextureCube() override;

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