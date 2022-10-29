#pragma once

#include "moci_core/moci_core.hpp"

namespace moci
{

struct FramebufferSpecs
{
    std::uint32_t width    = 0;
    std::uint32_t height   = 0;
    std::uint32_t samples  = 1;
    bool isSwapChainTarget = false;
};

class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    virtual void Bind()   = 0;
    virtual void Unbind() = 0;

    virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;

    [[nodiscard]] virtual auto GetColorAttachmentRendererID() const -> uint32_t    = 0;
    [[nodiscard]] virtual auto GetSpecification() const -> FramebufferSpecs const& = 0;
};
}  // namespace moci
