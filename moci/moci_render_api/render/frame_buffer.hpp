#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
struct FrameBufferFactory
{
    static Ref<Framebuffer> Make(FramebufferSpecs spec);
};

}  // namespace moci
