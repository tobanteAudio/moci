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

    // GLCall(glGenTextures(1, &colorAttachment_));
    // GLCall(glBindTexture(GL_TEXTURE_2D, colorAttachment_));
    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specs_.width, specs_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    // nullptr)); GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment_, 0));

    // GLCall(glGenTextures(1, &depthAttachment_));
    // GLCall(glBindTexture(GL_TEXTURE_2D, depthAttachment_));
    // GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT, specs_.width, specs_.height));
    // GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment_, 0));

    // MOCI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is
    // incomplete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // GLCall(glGenFramebuffers(1, &renderID_));
    // GLCall(glBindFramebuffer(GL_FRAMEBUFFER, renderID_));

    // GLCall(glGenTextures(1, &colorAttachment_));
    // GLCall(glBindTexture(GL_TEXTURE_2D, colorAttachment_));

    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    // // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specs_.width, specs_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    // GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    // // Build the texture that will serve as the depth attachment for the renderID_.
    // GLCall(glGenTextures(1, &depthAttachment_));
    // GLCall(glBindTexture(GL_TEXTURE_2D, depthAttachment_));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, specs_.width, specs_.height, 0, GL_DEPTH_COMPONENT,
    //                     GL_UNSIGNED_BYTE, NULL));
    // GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    // GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment_, 0));
    // GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment_, 0));
    // MOCI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "framebuffer is
    // incomplete"); GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLESFramebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, renderID_); }

void OpenGLESFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}  // namespace moci
