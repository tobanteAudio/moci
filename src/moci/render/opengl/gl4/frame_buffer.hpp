
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
        return _colorAttachment;
    }

    [[nodiscard]] auto getSpecification() const -> FramebufferSpecs const& override
    {
        return _specs;
    }

private:
    void invalidate();
    void deallocate();

    std::uint32_t _renderID        = 0;
    std::uint32_t _colorAttachment = 0;
    std::uint32_t _depthAttachment = 0;
    FramebufferSpecs _specs        = {};
};

}  // namespace moci

#endif
