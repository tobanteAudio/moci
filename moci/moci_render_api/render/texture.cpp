#include "moci_render_base/moci_render_base.hpp"

#include "renderer.hpp"

#include "moci_render_opengl/texture.hpp"
#include "moci_render_opengles/moci_render_opengles.hpp"

namespace moci
{

auto Texture2D::Create(std::string const& path) -> Ref<Texture2D>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLTexture2D>(path);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESTexture2D>(path);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto Texture2D::Create(Texture::Format format, uint32_t width, uint32_t height, void* data) -> Ref<Texture2D>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLTexture2D>(format, width, height, data);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESTexture2D>(format, width, height, data);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto TextureCube::Create(Vector<std::string> paths) -> Ref<TextureCube>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLTextureCube>(paths);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESTextureCube>(paths);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace moci