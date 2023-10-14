#pragma once

#include <moci/render/api.hpp>
#include <moci/render/buffer.hpp>
#include <moci/render/frame_buffer.hpp>
#include <moci/render/shader.hpp>
#include <moci/render/texture.hpp>

namespace moci {
struct RenderFactory
{
    static auto makeVertexBuffer(float* vertices, std::uint32_t size, bool dynamic = false)
        -> VertexBuffer*;
    static auto makeIndexBuffer(IndexBufferSpecs specs) -> IndexBuffer*;
    static auto makeFramebuffer(FramebufferSpecs spec) -> std::shared_ptr<Framebuffer>;
    static auto makeShader(std::string const& filepath) -> std::shared_ptr<Shader>;
    static auto makeShader(
        std::string const& name,
        std::string const& vertexSrc,
        std::string const& fragmentSrc
    ) -> std::shared_ptr<Shader>;
    static auto makeTexture2D(std::string const& path) -> std::shared_ptr<Texture2D>;
    static auto
    makeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data)
        -> std::shared_ptr<Texture2D>;
    static auto makeTextureCube(std::vector<std::string> const& paths)
        -> std::shared_ptr<TextureCube>;
    static auto makeVertexArray() -> std::shared_ptr<VertexArray>;
};

}  // namespace moci
