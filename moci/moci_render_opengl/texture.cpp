#include "moci_render_opengl/texture.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_core/benchmark/profile.hpp"
#include "moci_render_opengl/gl4.hpp"

#include <stb_image.h>

namespace moci
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
{

    m_InternalFormat = GL_RGBA8;
    m_DataFormat     = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(std::string path) : m_Path(std::move(path))
{

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    MOCI_CORE_ASSERT(data, "Failed to load image!");
    m_Width  = width;
    m_Height = height;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat     = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat     = GL_RGB;
    }

    m_InternalFormat = internalFormat;
    m_DataFormat     = dataFormat;

    MOCI_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(Texture::Format format, std::uint32_t width, std::uint32_t height, void* data)
    : m_Width(width), m_Height(height)
{
    IgnoreUnused(format);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, GL_RED, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RED, GL_UNSIGNED_BYTE, data);
}
OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

void OpenGLTexture2D::SetData(void* data, uint32_t size)
{

    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    MOCI_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const { glBindTextureUnit(slot, m_RendererID); }

OpenGLTextureCube::OpenGLTextureCube(Vector<std::string> paths) : paths_(std::move(paths))
{

    // Generate a texture object
    glGenTextures(1, &renderID_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_);

    // Set the filtering mode1
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // stbi_set_flip_vertically_on_load(1);
    for (GLuint i = 0; i < paths_.size(); i++)
    {
        int width      = 0;
        int height     = 0;
        int nrChannels = 0;
        auto* data     = stbi_load(paths_[i].c_str(), &width, &height, &nrChannels, 0);
        if (data != nullptr)
        {
            MOCI_CORE_INFO("stbi loaded: {} {}x{}", paths_[i].c_str(), width, height);
            auto const position = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
            glTexImage2D(position, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            MOCI_CORE_ERROR("stbi error: {}", paths_[i].c_str());
        }

        stbi_image_free(data);
    }
}

OpenGLTextureCube::~OpenGLTextureCube() { glDeleteTextures(1, &renderID_); }

void OpenGLTextureCube::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_);
}

void OpenGLTextureCube::Unbind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}  // namespace moci

#endif