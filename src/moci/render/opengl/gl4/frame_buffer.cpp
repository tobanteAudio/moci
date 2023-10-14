#include "frame_buffer.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

    #include "gl4.hpp"
    #include <moci/core/logging.hpp>

namespace moci {

OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecs spec) : _specs(spec) { invalidate(); }

OpenGLFramebuffer::~OpenGLFramebuffer() { deallocate(); }

void OpenGLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _renderID);
    glViewport(0, 0, _specs.width, _specs.height);
}

void OpenGLFramebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::resize(std::uint32_t width, std::uint32_t height)
{
    _specs.width  = width;
    _specs.height = height;
    invalidate();
}

void OpenGLFramebuffer::invalidate()
{
    if (_renderID != 0) {
        deallocate();
    }

    glCreateFramebuffers(1, &_renderID);
    glBindFramebuffer(GL_FRAMEBUFFER, _renderID);

    glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, _colorAttachment);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        _specs.width,
        _specs.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        _colorAttachment,
        0
    );

    glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachment);
    glBindTexture(GL_TEXTURE_2D, _depthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _specs.width, _specs.height);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        _depthAttachment,
        0
    );

    MOCI_CORE_ASSERT(
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is incomplete!"
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::deallocate()
{
    glDeleteFramebuffers(1, &_renderID);
    glDeleteTextures(1, &_colorAttachment);
    glDeleteTextures(1, &_depthAttachment);
}
}  // namespace moci

#endif
