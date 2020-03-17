#pragma once

#include "moci/render/texture.hpp"

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

    [[nodiscard]] auto GetWidth() const -> std::uint32_t override { return m_Width; }
    [[nodiscard]] auto GetHeight() const -> std::uint32_t override { return m_Height; }
    [[nodiscard]] auto GetID() const -> std::uint32_t override { return m_RendererID; }

    void Bind(std::uint32_t slot = 0) const override;
    void Unbind() const override;

private:
    std::string m_Path;
    std::int32_t m_Width {}, m_Height {}, m_BPP {};
    std::uint32_t m_RendererID {};
    std::uint8_t* m_LocalBuffer = nullptr;
};

}  // namespace moci