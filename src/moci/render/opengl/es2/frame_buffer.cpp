#include "frame_buffer.hpp"

#include "es2.hpp"

namespace moci {

OpenGLESFramebuffer::OpenGLESFramebuffer(FramebufferSpecs spec) : _specs(spec)
{
    invalidate();
}

OpenGLESFramebuffer::~OpenGLESFramebuffer() { deallocate(); }

void OpenGLESFramebuffer::bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _renderID));
    GLCall(glViewport(0, 0, _specs.width, _specs.height));
}

void OpenGLESFramebuffer::unbind() { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

void OpenGLESFramebuffer::resize(std::uint32_t width, std::uint32_t height)
{
    _specs.width  = width;
    _specs.height = height;
    invalidate();
}

void OpenGLESFramebuffer::invalidate()
{
    if (_renderID != 0) {
        deallocate();
    }

    GLCall(glGenFramebuffers(1, &_renderID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _renderID));

    GLCall(glGenTextures(1, &_colorAttachment));
    GLCall(glBindTexture(GL_TEXTURE_2D, _colorAttachment));
    GLCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        _specs.width,
        _specs.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    ));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        _colorAttachment,
        0
    ));

    MOCI_CORE_ASSERT(
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is incomplete!"
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLESFramebuffer::deallocate()
{
    GLCall(glDeleteFramebuffers(1, &_renderID));
    GLCall(glDeleteTextures(1, &_colorAttachment));
}
}  // namespace moci
