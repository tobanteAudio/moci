#include "frame_buffer.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"
#include "moci_core/moci_core.hpp"

namespace moci
{

OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecs spec) : specs_(std::move(spec)) { invalidate(); }

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &renderID_);
    glDeleteTextures(1, &colorAttachment_);
    glDeleteTextures(1, &depthAttachment_);
}

void OpenGLFramebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, renderID_); }

void OpenGLFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::Resize(std::uint32_t width, std::uint32_t height) { }

void OpenGLFramebuffer::invalidate()
{
    glCreateFramebuffers(1, &renderID_);
    glBindFramebuffer(GL_FRAMEBUFFER, renderID_);

    glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment_);
    glBindTexture(GL_TEXTURE_2D, colorAttachment_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specs_.width, specs_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment_, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment_);
    glBindTexture(GL_TEXTURE_2D, depthAttachment_);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, specs_.width, specs_.height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment_, 0);

    MOCI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}  // namespace moci

#endif