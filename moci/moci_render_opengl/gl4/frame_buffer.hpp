
#pragma once

#include "moci/render/frame_buffer.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

namespace moci
{

class OpenGLFramebuffer : public Framebuffer
{
public:
    explicit OpenGLFramebuffer(FramebufferSpecs spec);
    ~OpenGLFramebuffer() override;

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
    std::uint32_t depthAttachment_ = 0;
    FramebufferSpecs specs_        = {};
};

}  // namespace moci

#endif