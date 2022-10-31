#pragma once

#include <moci/render/api.hpp>
#include <moci/render/buffer.hpp>
#include <moci/render/frame_buffer.hpp>
#include <moci/render/shader.hpp>
#include <moci/render/texture.hpp>

namespace moci
{
struct RenderFactory
{
    static auto makeVertexBuffer(float* vertices, std::uint32_t size, bool dynamic = false) -> VertexBuffer*;
    static auto makeIndexBuffer(IndexBufferSpecs specs) -> IndexBuffer*;
    static auto makeFramebuffer(FramebufferSpecs spec) -> Ref<Framebuffer>;
    static auto makeShader(const std::string& filepath) -> Ref<Shader>;
    static auto makeShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
        -> Ref<Shader>;
    static auto makeTexture2D(std::string const& path) -> Ref<Texture2D>;
    static auto makeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data) -> Ref<Texture2D>;
    static auto makeTextureCube(const Vector<std::string>& paths) -> Ref<TextureCube>;
    static auto makeVertexArray() -> Ref<VertexArray>;
};

}  // namespace moci
