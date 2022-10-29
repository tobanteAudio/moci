#pragma once

#include <cstdint>
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

    virtual void bind()   = 0;
    virtual void unbind() = 0;

    virtual void resize(std::uint32_t width, std::uint32_t height) = 0;

    [[nodiscard]] virtual auto getColorAttachmentRendererId() const -> uint32_t    = 0;
    [[nodiscard]] virtual auto getSpecification() const -> FramebufferSpecs const& = 0;
};
}  // namespace moci
