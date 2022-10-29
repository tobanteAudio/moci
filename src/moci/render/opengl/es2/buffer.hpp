#pragma once

#include "moci/core/span.hpp"
#include "moci/render/buffer.hpp"

#include <cstddef>

namespace moci
{

class OpenGLESVertexBuffer : public VertexBuffer
{
public:
    OpenGLESVertexBuffer(float* vertices, uint32_t size, bool dynamic);
    ~OpenGLESVertexBuffer() override;

    void bind() const override;
    void unbind() const override;
    auto uploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void override;
    [[nodiscard]] auto getLayout() const -> const BufferLayout& override { return _m_Layout; }
    void setLayout(const BufferLayout& layout) override { _m_Layout = layout; }

private:
    uint32_t _m_RendererID {};
    BufferLayout _m_Layout;
};

class OpenGLESIndexBuffer : public IndexBuffer
{
public:
    explicit OpenGLESIndexBuffer(IndexBufferSpecs specs);
    ~OpenGLESIndexBuffer() override;

    void bind() const override;
    void unbind() const override;
    auto uploadData(std::uint32_t offset, Span<std::uint32_t> indices) const -> void override;
    [[nodiscard]] auto getCount() const -> uint32_t override { return _specs.count; }

private:
    [[nodiscard]] auto convertToUnsignedShorts(Span<std::uint32_t> indices) const -> Span<std::uint16_t>;

    IndexBufferSpecs _specs                    = {};
    uint32_t _m_RendererID                     = {};
    Scope<Vector<std::uint16_t>> _indicesShort = makeScope<Vector<std::uint16_t>>();
};

}  // namespace moci