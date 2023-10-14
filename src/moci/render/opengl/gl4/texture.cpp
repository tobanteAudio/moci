#include "texture.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

    #include "gl4.hpp"

    #include <stb_image.h>

    #include <cstddef>

namespace moci {

OpenGLTexture2D::OpenGLTexture2D(std::uint32_t width, std::uint32_t height)
    : _width{width}
    , _height{height}
    , _channels{4}
    , _internalFormat{GL_RGBA8}
    , _dataFormat{GL_RGBA}
{
    createTexture();
    setFilters();
}

OpenGLTexture2D::OpenGLTexture2D(std::string path) : _path(std::move(path))
{

    stbi_set_flip_vertically_on_load(1);

    auto width    = 0;
    auto height   = 0;
    auto channels = 0;
    auto* data    = stbi_load(_path.c_str(), &width, &height, &channels, 4);
    MOCI_CORE_ASSERT(data, "Failed to load image!");

    _width          = static_cast<std::uint32_t>(width);
    _height         = static_cast<std::uint32_t>(height);
    _channels       = static_cast<std::uint32_t>(channels);
    _internalFormat = GL_RGBA8;
    _dataFormat     = GL_RGBA;

    // if (channels == 4)
    // {
    //     internalFormat_ = GL_RGBA8;
    //     dataFormat_     = GL_RGBA;
    // }
    // else if (channels == 3)
    // {
    //     internalFormat_ = GL_RGB8;
    //     dataFormat_     = GL_RGB;
    // }

    MOCI_CORE_ASSERT(_internalFormat & _dataFormat, "Format not supported!");

    createTexture();
    setFilters();
    setData(std::span<std::uint8_t>{
        reinterpret_cast<std::uint8_t*>(data),
        static_cast<size_t>(_width * _height * _channels)
    });

    stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(
    Texture::Format format,
    std::uint32_t width,
    std::uint32_t height,
    void* data
)
    : _width(width)
    , _height(height)
{
    ignoreUnused(format);
    createTexture();
    setFilters();
    setData(std::span<std::uint8_t>{
        reinterpret_cast<std::uint8_t*>(data),
        static_cast<size_t>(_width * _height * _channels)
    });
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &_renderID); }

void OpenGLTexture2D::setData(std::span<std::uint8_t> data) const
{
    MOCI_CORE_ASSERT(
        data.size() == static_cast<unsigned long>(_width * _height * _channels),
        "Data must be entire texture!"
    );
    glTextureSubImage2D(
        _renderID,
        0,
        0,
        0,
        _width,
        _height,
        _dataFormat,
        GL_UNSIGNED_BYTE,
        data.data()
    );
}

void OpenGLTexture2D::bind(uint32_t slot) const { glBindTextureUnit(slot, _renderID); }

void OpenGLTexture2D::createTexture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &_renderID);
    glTextureStorage2D(_renderID, 1, _internalFormat, _width, _height);
}

void OpenGLTexture2D::setFilters() const
{
    glTextureParameteri(_renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(_renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(_renderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(_renderID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTextureCube::OpenGLTextureCube(std::vector<std::string> paths)
    : _paths(std::move(paths))
{

    // Generate a texture object
    glGenTextures(1, &_renderID);

    // bind the texture object
    glBindTexture(GL_TEXTURE_CUBE_MAP, _renderID);

    // Set the filtering mode1
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // stbi_set_flip_vertically_on_load(1);
    for (GLuint i = 0; i < _paths.size(); i++) {
        int width      = 0;
        int height     = 0;
        int nrChannels = 0;
        auto* data     = stbi_load(_paths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data != nullptr) {
            MOCI_CORE_INFO("stbi loaded: {} {}x{}", _paths[i].c_str(), width, height);
            auto const position = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
            glTexImage2D(
                position,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
        } else {
            MOCI_CORE_ERROR("stbi error: {}", _paths[i].c_str());
        }

        stbi_image_free(data);
    }
}

OpenGLTextureCube::~OpenGLTextureCube() { glDeleteTextures(1, &_renderID); }

void OpenGLTextureCube::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _renderID);
}

void OpenGLTextureCube::unbind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}  // namespace moci

#endif
