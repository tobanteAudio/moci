
#pragma once

#include <moci/render/frame_buffer.hpp>

namespace moci
{

class OpenGLESFramebuffer : public Framebuffer
{
public:
    explicit OpenGLESFramebuffer(FramebufferSpecs spec);
    ~OpenGLESFramebuffer() override;

    void bind() override;
    void unbind() override;

    void resize(std::uint32_t width, std::uint32_t height) override;

    [[nodiscard]] auto getColorAttachmentRendererId() const -> std::uint32_t override { return _colorAttachment; }
    [[nodiscard]] auto getSpecification() const -> const FramebufferSpecs& override { return _specs; }

private:
    void invalidate();
    void deallocate();

    std::uint32_t _renderID        = 0;
    std::uint32_t _colorAttachment = 0;
    FramebufferSpecs _specs        = {};
};

}  // namespace moci
