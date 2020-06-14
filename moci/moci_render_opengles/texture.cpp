#include "moci_render_opengles/texture.hpp"

#include <utility>

#include "stb_image.h"
#include "stb_image_resize.h"

#include "moci_core/core/logging.hpp"
#include "moci_core/core/preprocessor.hpp"

#include "moci_render_opengles/es2.hpp"

namespace moci
{

OpenGLESTexture2D::OpenGLESTexture2D(std::string path) : path_(std::move(path))
{
    stbi_set_flip_vertically_on_load(1);
    data_ = stbi_load(path_.c_str(), &width_, &height_, &bpp_, 4);
    if (data_ == nullptr)
    {
        MOCI_CORE_ERROR("stbi error");
    }

    // Use tightly packed data
    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    // Generate a texture object
    GLCall(glGenTextures(1, &renderID_));

    // Bind the texture object
    GLCall(glBindTexture(GL_TEXTURE_2D, renderID_));

    // Set the filtering mode1
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Load the texture
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_));

    if (data_ != nullptr)
    {
        stbi_image_free(data_);
    }
}

OpenGLESTexture2D::OpenGLESTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data)
{
    IgnoreUnused(format);

    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction

    GLCall(glGenTextures(1, &renderID_));
    GLCall(glBindTexture(GL_TEXTURE_2D, renderID_));

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

OpenGLESTexture2D::~OpenGLESTexture2D() { GLCall(glDeleteTextures(1, &renderID_)); }

void OpenGLESTexture2D::Bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, renderID_));
}

void OpenGLESTexture2D::Unbind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGLESTextureCube::OpenGLESTextureCube(Vector<std::string> paths) : paths_(std::move(paths))
{

    // Generate a texture object
    GLCall(glGenTextures(1, &renderID_));

    // Bind the texture object
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_));

    // Set the filtering mode1
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    for (GLuint i = 0; i < paths_.size(); i++)
    {
        int width       = 0;
        int height      = 0;
        int numChannels = 0;
        auto* data      = stbi_load(paths_[i].c_str(), &width, &height, &numChannels, 0);
        if (data != nullptr)
        {
            MOCI_CORE_INFO("stbi loaded: {} {}x{}", paths_[i].c_str(), width, height);
            auto const pos = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;

            // Resize on ARM devices with little VRAM. (RPi3 for example)
#if defined(MOCI_ARM)
            if (width > 1024)
            {
                auto const newSize = 1024;
                MOCI_CORE_INFO("    stbi resize: FROM: {}x{} TO: {}x{}", width, height, newSize, newSize);
                Vector<stbi_uc> outBuffer {};
                outBuffer.resize(newSize * newSize * numChannels);
                if (stbir_resize_uint8(data, width, height, 0, outBuffer.data(), newSize, newSize, 0, numChannels) == 0)
                {
                    MOCI_ERROR("    stbi resize error");
                }

                GLCall(glTexImage2D(pos, 0, GL_RGB, newSize, newSize, 0, GL_RGB, GL_UNSIGNED_BYTE, outBuffer.data()));
            }
            else
            {

                GLCall(glTexImage2D(pos, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
            }

#else
            GLCall(glTexImage2D(pos, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
#endif
        }
        else
        {
            MOCI_CORE_ERROR("stbi error: {}", paths_[i]);
        }

        stbi_image_free(data);
    }
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