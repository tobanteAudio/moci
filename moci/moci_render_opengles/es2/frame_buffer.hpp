
#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{

class OpenGLESFramebuffer : public Framebuffer
{
public:
    explicit OpenGLESFramebuffer(FramebufferSpecs spec);
    ~OpenGLESFramebuffer() override;

    void Bind() override;
    void Unbind() override;

    void Resize(std::uint32_t width, std::uint32_t height) override;

    [[nodiscard]] auto GetColorAttachmentRendererID() const -> std::uint32_t override { return colorAttachment_; }
    [[nodiscard]] auto GetSpecification() const -> const FramebufferSpecs& override { return specs_; }

private:
    void invalidate();
    void deallocate();

    std::uint32_t renderID_        = 0;
    std::uint32_t colorAttachment_ = 0;
    FramebufferSpecs specs_        = {};
};

}  // namespace moci
