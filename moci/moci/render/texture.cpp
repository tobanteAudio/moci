#include "texture.hpp"

#include "renderer.hpp"

#include "moci/render/opengl/es2/texture.hpp"

namespace moci
{

auto Texture2D::Create(std::string const& path) -> std::shared_ptr<Texture2D>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        // case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESTexture2D>(path);
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto Texture2D::Create(Texture::Format format, uint32_t width, uint32_t height, void* data)
    -> std::shared_ptr<Texture2D>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        // case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESTexture2D>(format, width, height, data);
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci