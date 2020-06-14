#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
struct RenderFactory
{
    static Ref<Framebuffer> MakeFramebuffer(FramebufferSpecs spec);
    static Ref<Shader> MakeShader(std::string const& filepath);
    static Ref<Shader> MakeShader(std::string const& name, std::string const& vertexSrc,
                                  std::string const& fragmentSrc);

    static Ref<Texture2D> MakeTexture2D(std::string const& path);
    static Ref<Texture2D> MakeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data);
    static Ref<TextureCube> MakeTextureCube(Vector<std::string> paths);
};

}  // namespace moci
