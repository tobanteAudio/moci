#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
struct RenderFactory
{
    static VertexBuffer* MakeVertexBuffer(float* vertices, std::uint32_t size, bool dynamic = false);
    static IndexBuffer* MakeIndexBuffer(IndexBufferSpecs specs);
    static Ref<Framebuffer> MakeFramebuffer(FramebufferSpecs spec);
    static Ref<Shader> MakeShader(const std::string& filepath);
    static Ref<Shader> MakeShader(std::string const& name, std::string const& vertexSrc,
                                  std::string const& fragmentSrc);
    static Ref<Texture2D> MakeTexture2D(std::string const& path);
    static Ref<Texture2D> MakeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data);
    static Ref<TextureCube> MakeTextureCube(const Vector<std::string>& paths);
    static Ref<VertexArray> MakeVertexArray();
};

}  // namespace moci
