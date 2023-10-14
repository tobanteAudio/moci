#pragma once

#include <moci/core/benchmark/profile.hpp>
#include <moci/core/logging.hpp>
#include <moci/graphics/color.hpp>
#include <moci/render/api.hpp>

namespace moci
{

class RenderCommand
{
public:
    inline static void init()
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->init();
    }

    inline static void setViewport(Rectangle<uint32_t> viewport)
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->setViewport(viewport);
    }

    inline static void setClearColor(ColorRGBA32 const& color)
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->setClearColor(color);
    }

    inline static void clear()
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->clear();
    }

    inline static void drawArrays(RenderDrawMode mode, uint32_t first, uint32_t count)
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->drawArrays(mode, first, count);
    }

    inline static void drawIndexed(RenderDrawMode mode, uint32_t count, void* indices)
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->drawIndexed(mode, count, indices);
    }

    inline static void drawIndexed(RenderDrawMode mode, std::shared_ptr<VertexArray> const& vertexArray)
    {
        MOCI_PROFILE_FUNCTION();
        sRendererApi->drawIndexed(mode, vertexArray);
    }

    inline static auto maxTextureSize() -> std::uint32_t { return sRendererApi->maxTextureSize(); }
    inline static auto maxTextureUnits() -> std::uint32_t { return sRendererApi->maxTextureUnits(); }
    inline static auto maxVertexAttributes() -> std::uint32_t { return sRendererApi->maxVertexAttributes(); }
    inline static auto maxUniformVectors() -> std::uint32_t { return sRendererApi->maxUniformVectors(); }

private:
    static std::unique_ptr<RendererAPI> sRendererApi;
};

}  // namespace moci
