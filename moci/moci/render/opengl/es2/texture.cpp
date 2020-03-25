#include "moci/render/opengl/es2/texture.hpp"

#include <utility>

#include "stb_image.h"

#include "moci/core/logging.hpp"

#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{

OpenGLESTexture2D::OpenGLESTexture2D(std::string path) : m_Path(std::move(path))
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if (m_LocalBuffer == nullptr)
    {
        MOCI_CORE_ERROR("stbi error");
    }

    // 2 x 2 Image, 4 bytes per pixel(R, G, B,A)
    // GLubyte pixels[] = {
    //     0,   255, 0, 255,  // Green
    //     255, 255, 0, 255,  // Yellow
    //     255, 0,   0, 255,  // Red
    //     0,   0,   0, 255,  // Black
    // };

    // Use tightly packed data
    // GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    // Generate a texture object
    GLCall(glGenTextures(1, &m_RendererID));

    // Bind the texture object
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // Set the filtering mode1
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    // GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    // Load the texture
    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_width, fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

    if (m_LocalBuffer != nullptr)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

OpenGLESTexture2D::OpenGLESTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data)
{
    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // GL_LUMINANCE: Was GL_RED, but thats not available on RPi3
    GLCall(                    //
        glTexImage2D(          //
            GL_TEXTURE_2D,     //
            0,                 //
            GL_LUMINANCE,      //
            width,             //
            height,            //
            0,                 //
            GL_LUMINANCE,      //
            GL_UNSIGNED_BYTE,  //
            data               //
            )                  //
    );

    // Set texture options
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

OpenGLESTexture2D::~OpenGLESTexture2D() { GLCall(glDeleteTextures(1, &m_RendererID)); }

void OpenGLESTexture2D::Bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void OpenGLESTexture2D::Unbind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGLESTextureCube::OpenGLESTextureCube(std::vector<std::string> paths) : paths_(std::move(paths))
{

    // Generate a texture object
    GLCall(glGenTextures(1, &renderID_));

    // Bind the texture object
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_));

    stbi_set_flip_vertically_on_load(1);
    for (GLuint i = 0; i < paths_.size(); i++)
    {
        unsigned char* data;
        int width, height, nrChannels;
        data = stbi_load(paths_[i].c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            MOCI_CORE_ERROR("stbi error");
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        if (data != nullptr)
        {
            stbi_image_free(data);
        }
    }

    // Set the filtering mode1
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

OpenGLESTextureCube::~OpenGLESTextureCube() { GLCall(glDeleteTextures(1, &renderID_)); }

void OpenGLESTextureCube::Bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_));
}

void OpenGLESTextureCube::Unbind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
}  // namespace moci