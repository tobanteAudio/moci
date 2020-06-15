#include "frame_buffer.hpp"

#include "es2.hpp"
#include "moci_core/moci_core.hpp"

namespace moci
{

OpenGLESFramebuffer::OpenGLESFramebuffer(FramebufferSpecs spec) : specs_(std::move(spec)) { Invalidate(); }

OpenGLESFramebuffer::~OpenGLESFramebuffer() { GLCall(glDeleteFramebuffers(1, &renderID_)); }

void OpenGLESFramebuffer::Invalidate()
{
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

void OpenGLESFramebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, renderID_); }

void OpenGLESFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}  // namespace moci
