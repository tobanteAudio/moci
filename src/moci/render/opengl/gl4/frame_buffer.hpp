
#pragma once

#include <moci/render/frame_buffer.hpp>

#if defined(MOCI_API_OPENGL_MODERN)

namespace moci {

class OpenGLFramebuffer : public Framebuffer
{
public:
    explicit OpenGLFramebuffer(FramebufferSpecs spec);
    ~OpenGLFramebuffer() override;

    void bind() override;
    void unbind() override;

    void resize(std::uint32_t width, std::uint32_t height) override;

    [[nodiscard]] auto getColorAttachmentRendererId() const -> std::uint32_t override
    {
        return colorAttachment_;
    }

    [[nodiscard]] auto getSpecification() const -> FramebufferSpecs const& override
    {
        return specs_;
    }

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
