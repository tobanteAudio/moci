
#pragma once

#include "moci_render_base/moci_render_base.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

namespace moci
{

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(FramebufferSpecs spec);
    ~OpenGLFramebuffer() override;

    void Bind() override;
    void Unbind() override;

    void Resize(std::uint32_t width, std::uint32_t height) override;

    std::uint32_t GetColorAttachmentRendererID() const override { return colorAttachment_; }
    const FramebufferSpecs& GetSpecification() const override { return specs_; }

private:
    void invalidate();

    std::uint32_t renderID_        = 0;
    std::uint32_t colorAttachment_ = 0;
    std::uint32_t depthAttachment_ = 0;
    FramebufferSpecs specs_        = {};
};

}  // namespace moci

#endif