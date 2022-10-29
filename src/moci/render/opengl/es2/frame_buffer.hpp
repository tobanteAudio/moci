
#pragma once

#include "moci/render/frame_buffer.hpp"

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

    [[nodiscard]] auto getColorAttachmentRendererId() const -> std::uint32_t override { return colorAttachment_; }
    [[nodiscard]] auto getSpecification() const -> const FramebufferSpecs& override { return specs_; }

private:
    void invalidate();
    void deallocate();

    std::uint32_t renderID_        = 0;
    std::uint32_t colorAttachment_ = 0;
    FramebufferSpecs specs_        = {};
};

}  // namespace moci
