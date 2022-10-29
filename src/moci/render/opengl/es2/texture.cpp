#include "texture.hpp"

#include "moci/core/logging.hpp"
#include "moci/core/preprocessor.hpp"
#include "moci/render/opengl/es2/es2.hpp"

#include "stb_image.h"
#include "stb_image_resize.h"

#include <utility>

namespace moci
{

OpenGLESTexture2D::OpenGLESTexture2D(std::string path) : _path(std::move(path))
{
    stbi_set_flip_vertically_on_load(1);
    _data = stbi_load(_path.c_str(), &_width, &_height, &_bpp, 4);
    if (_data == nullptr) { MOCI_CORE_ERROR("stbi error"); }

    createTexture();
    setFilterModes();
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data));

    if (_data != nullptr) { stbi_image_free(_data); }
}

OpenGLESTexture2D::OpenGLESTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data)
{
    ignoreUnused(format);
    createTexture();
    setFilterModes();

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
}

OpenGLESTexture2D::~OpenGLESTexture2D() { GLCall(glDeleteTextures(1, &_renderID)); }

void OpenGLESTexture2D::bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, _renderID));
}

void OpenGLESTexture2D::unbind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGLESTexture2D::createTexture()
{
    // Disable byte-alignment restriction
    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GLCall(glGenTextures(1, &_renderID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _renderID));
}
void OpenGLESTexture2D::setFilterModes()
{
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

OpenGLESTextureCube::OpenGLESTextureCube(Vector<std::string> paths) : _paths(std::move(paths))
{

    // Generate a texture object
    GLCall(glGenTextures(1, &_renderID));

    // Bind the texture object
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _renderID));

    // Set the filtering mode1
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    for (GLuint i = 0; i < _paths.size(); i++)
    {
        int width       = 0;
        int height      = 0;
        int numChannels = 0;
        auto* data      = stbi_load(_paths[i].c_str(), &width, &height, &numChannels, 0);
        if (data != nullptr)
        {
            MOCI_CORE_INFO("stbi loaded: {} {}x{}", _paths[i].c_str(), width, height);
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
            else { GLCall(glTexImage2D(pos, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data)); }

#else
            GLCall(glTexImage2D(pos, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
#endif
        }
        else { MOCI_CORE_ERROR("stbi error: {}", _paths[i]); }

        stbi_image_free(data);
    }
}

OpenGLESTextureCube::~OpenGLESTextureCube() { GLCall(glDeleteTextures(1, &_renderID)); }

void OpenGLESTextureCube::bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _renderID));
}

void OpenGLESTextureCube::unbind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
}  // namespace moci