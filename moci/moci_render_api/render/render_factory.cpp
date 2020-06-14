#include "render_factory.hpp"

namespace moci
{
Ref<Framebuffer> RenderFactory::MakeFramebuffer(FramebufferSpecs spec)
{
    IgnoreUnused(spec);
    return nullptr;
}

}  // namespace moci