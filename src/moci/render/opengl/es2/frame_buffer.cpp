#include "frame_buffer.hpp"

#include "es2.hpp"

namespace moci
{

OpenGLESFramebuffer::OpenGLESFramebuffer(FramebufferSpecs spec) : specs_(spec) { invalidate(); }

OpenGLESFramebuffer::~OpenGLESFramebuffer() { deallocate(); }

void OpenGLESFramebuffer::bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, renderID_));
    GLCall(glViewport(0, 0, specs_.width, specs_.height));
}

void OpenGLESFramebuffer::unbind() { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

void OpenGLESFramebuffer::resize(std::uint32_t width, std::uint32_t height)
{
    specs_.width  = width;
    specs_.height = height;
    invalidate();
}

void OpenGLESFramebuffer::invalidate()
{
    if (renderID_ != 0) { deallocate(); }

    GLCall(glGenFramebuffers(1, &renderID_));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, renderID_));

    GLCall(glGenTextures(1, &colorAttachment_));
    GLCall(glBindTexture(GL_TEXTURE_2D, colorAttachment_));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specs_.width, specs_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment_, 0));

    MOCI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void OpenGLESFramebuffer::deallocate()
{
    GLCall(glDeleteFramebuffers(1, &renderID_));
    GLCall(glDeleteTextures(1, &colorAttachment_));
}
}  // namespace moci
