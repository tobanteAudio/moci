#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
struct RenderFactory
{
    static auto MakeVertexBuffer(float* vertices, std::uint32_t size, bool dynamic = false) -> VertexBuffer*;
    static auto MakeIndexBuffer(IndexBufferSpecs specs) -> IndexBuffer*;
    static auto MakeFramebuffer(FramebufferSpecs spec) -> Ref<Framebuffer>;
    static auto MakeShader(const std::string& filepath) -> Ref<Shader>;
    static auto MakeShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
        -> Ref<Shader>;
    static auto MakeTexture2D(std::string const& path) -> Ref<Texture2D>;
    static auto MakeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data) -> Ref<Texture2D>;
    static auto MakeTextureCube(const Vector<std::string>& paths) -> Ref<TextureCube>;
    static auto MakeVertexArray() -> Ref<VertexArray>;
};

}  // namespace moci
