#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
struct RenderFactory
{
    static Ref<Framebuffer> MakeFramebuffer(FramebufferSpecs spec);
};

}  // namespace moci
