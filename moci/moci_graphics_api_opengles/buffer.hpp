#pragma once

#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

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
    OpenGLESIndexBuffer(uint32_t* indices, uint32_t count, bool dynamic);
    ~OpenGLESIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;
    auto UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void override;
    [[nodiscard]] auto GetCount() const -> uint32_t override { return m_Count; }

private:
    uint32_t m_RendererID {};
    uint32_t m_Count;
};

}  // namespace moci