#include "texture.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

    #include "gl4.hpp"

    #include <stb_image.h>

    #include <cstddef>

namespace moci {

OpenGLTexture2D::OpenGLTexture2D(std::uint32_t width, std::uint32_t height)
    : width_{width}
    , height_{height}
    , channels_{4}
    , internalFormat_{GL_RGBA8}
    , dataFormat_{GL_RGBA}
{
    createTexture();
    setFilters();
}

OpenGLTexture2D::OpenGLTexture2D(std::string path) : path_(std::move(path))
{

    stbi_set_flip_vertically_on_load(1);

    auto width    = 0;
    auto height   = 0;
    auto channels = 0;
    auto* data    = stbi_load(path_.c_str(), &width, &height, &channels, 4);
    MOCI_CORE_ASSERT(data, "Failed to load image!");

    width_          = static_cast<std::uint32_t>(width);
    height_         = static_cast<std::uint32_t>(height);
    channels_       = static_cast<std::uint32_t>(channels);
    internalFormat_ = GL_RGBA8;
    dataFormat_     = GL_RGBA;

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

    MOCI_CORE_ASSERT(internalFormat_ & dataFormat_, "Format not supported!");

    createTexture();
    setFilters();
    setData(std::span<std::uint8_t>{
        reinterpret_cast<std::uint8_t*>(data),
        static_cast<size_t>(width_ * height_ * channels_)
    });

    stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(
    Texture::Format format,
    std::uint32_t width,
    std::uint32_t height,
    void* data
)
    : width_(width)
    , height_(height)
{
    ignoreUnused(format);
    createTexture();
    setFilters();
    setData(std::span<std::uint8_t>{
        reinterpret_cast<std::uint8_t*>(data),
        static_cast<size_t>(width_ * height_ * channels_)
    });
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &renderID_); }

void OpenGLTexture2D::setData(std::span<std::uint8_t> data) const
{
    MOCI_CORE_ASSERT(
        data.size() == static_cast<unsigned long>(width_ * height_ * channels_),
        "Data must be entire texture!"
    );
    glTextureSubImage2D(
        renderID_,
        0,
        0,
        0,
        width_,
        height_,
        dataFormat_,
        GL_UNSIGNED_BYTE,
        data.data()
    );
}

void OpenGLTexture2D::bind(uint32_t slot) const { glBindTextureUnit(slot, renderID_); }

void OpenGLTexture2D::createTexture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &renderID_);
    glTextureStorage2D(renderID_, 1, internalFormat_, width_, height_);
}

void OpenGLTexture2D::setFilters() const
{
    glTextureParameteri(renderID_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderID_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(renderID_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(renderID_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTextureCube::OpenGLTextureCube(std::vector<std::string> paths)
    : paths_(std::move(paths))
{

    // Generate a texture object
    glGenTextures(1, &renderID_);

    // bind the texture object
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_);

    // Set the filtering mode1
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // stbi_set_flip_vertically_on_load(1);
    for (GLuint i = 0; i < paths_.size(); i++) {
        int width      = 0;
        int height     = 0;
        int nrChannels = 0;
        auto* data     = stbi_load(paths_[i].c_str(), &width, &height, &nrChannels, 0);
        if (data != nullptr) {
            MOCI_CORE_INFO("stbi loaded: {} {}x{}", paths_[i].c_str(), width, height);
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
            MOCI_CORE_ERROR("stbi error: {}", paths_[i].c_str());
        }

        stbi_image_free(data);
    }
}

OpenGLTextureCube::~OpenGLTextureCube() { glDeleteTextures(1, &renderID_); }

void OpenGLTextureCube::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderID_);
}

void OpenGLTextureCube::unbind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}  // namespace moci

#endif
