#pragma once

#include "moci_render_base/moci_render_base.hpp"

#include <cstddef>

namespace moci
{

class OpenGLESVertexBuffer : public VertexBuffer
{
public:
    OpenGLESVertexBuffer(float* vertices, uint32_t size, bool dynamic);
    ~OpenGLESVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;
    auto UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void override;
    [[nodiscard]] auto GetLayout() const -> const BufferLayout& override { return m_Layout; }
    void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
    uint32_t m_RendererID {};
    BufferLayout m_Layout;
};

class OpenGLESIndexBuffer : public IndexBuffer
{
public:
    OpenGLESIndexBuffer(IndexBufferSpecs specs);
    ~OpenGLESIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;
    auto UploadData(std::uint32_t offset, Span<std::uint32_t> indices) const -> void override;
    [[nodiscard]] auto GetCount() const -> uint32_t override { return specs_.count; }

private:
    Vector<std::uint16_t> convertToUnsignedShorts(Span<std::uint32_t> indices) const;

    IndexBufferSpecs specs_ = {};
    uint32_t m_RendererID   = {};
};

}  // namespace moci