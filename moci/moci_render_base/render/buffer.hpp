#pragma once

#include "moci_core/moci_core.hpp"

#include <cstdint>

#include <initializer_list>
#include <utility>

namespace moci
{

enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static auto ShaderDataTypeSize(ShaderDataType type) -> std::uint32_t
{
    switch (type)
    {
        case ShaderDataType::Float: return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;
        case ShaderDataType::Mat3: return 4 * 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        case ShaderDataType::Int: return 4;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;
        case ShaderDataType::Bool: return 1;
        case ShaderDataType::None: MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!"); return 0;
    }

    MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

struct BufferElement
{
    std::string Name;
    ShaderDataType Type;
    std::uint32_t Size;
    size_t Offset;
    bool Normalized;

    // BufferElement() = default;

    BufferElement(ShaderDataType type, std::string name, bool normalized = false)
        : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
    {
    }

    [[nodiscard]] auto GetComponentCount() const -> std::uint32_t
    {
        switch (Type)
        {
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::None: MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!"); return 0;
        }

        MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) { CalculateOffsetsAndStride(); }

    [[nodiscard]] inline auto GetStride() const -> std::uint32_t { return m_Stride; }
    [[nodiscard]] inline auto GetElements() const -> Vector<BufferElement> const& { return m_Elements; }

    auto begin() -> Vector<BufferElement>::iterator { return m_Elements.begin(); }
    auto end() -> Vector<BufferElement>::iterator { return m_Elements.end(); }
    [[nodiscard]] auto begin() const -> Vector<BufferElement>::const_iterator { return m_Elements.begin(); }
    [[nodiscard]] auto end() const -> Vector<BufferElement>::const_iterator { return m_Elements.end(); }

private:
    void CalculateOffsetsAndStride()
    {
        size_t offset = 0;
        m_Stride      = 0;
        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    Vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual auto Bind() const -> void                                                                 = 0;
    virtual auto Unbind() const -> void                                                               = 0;
    [[nodiscard]] virtual auto GetLayout() const -> BufferLayout const&                               = 0;
    virtual auto SetLayout(BufferLayout const& layout) -> void                                        = 0;
    virtual auto UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void = 0;
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const                                                             = 0;
    virtual void Unbind() const                                                           = 0;
    [[nodiscard]] virtual auto GetCount() const -> std::uint32_t                          = 0;
    virtual auto UploadData(std::uint32_t offset, Span<std::uint32_t> data) const -> void = 0;
};

}  // namespace moci