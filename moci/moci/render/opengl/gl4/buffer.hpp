#pragma once

#include "moci/render/buffer.hpp"

namespace moci
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(float* vertices, uint32_t size, bool dynamic = false);
    virtual ~OpenGLVertexBuffer();

    void Bind() const override;
    void Unbind() const override;

    const BufferLayout& GetLayout() const override { return m_Layout; }
    void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
    void UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const override;

private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count, bool dynamic = false);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t GetCount() const override { return m_Count; }
    void UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const override;

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};

}  // namespace moci