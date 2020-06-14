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
};

}  // namespace moci
