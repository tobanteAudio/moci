#pragma once

#include <moci/core/logging.hpp>

#include <cstdint>
#include <initializer_list>
#include <span>
#include <utility>
#include <vector>

namespace moci {

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

static auto shaderDataTypeSize(ShaderDataType type) -> std::uint32_t
{
    switch (type) {
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
        case ShaderDataType::None:
            MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
    }

    MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

struct BufferElement
{
    std::string Name;
    ShaderDataType Type;
    std::uint32_t Size;
    size_t Offset{0};
    bool Normalized;

    // BufferElement() = default;

    BufferElement(ShaderDataType type, std::string name, bool normalized = false)
        : Name(std::move(name))
        , Type(type)
        , Size(shaderDataTypeSize(type))
        , Normalized(normalized)
    {}

    [[nodiscard]] auto getComponentCount() const -> std::uint32_t
    {
        switch (Type) {
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
            case ShaderDataType::None:
                MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
        }

        MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(std::initializer_list<BufferElement> elements) : _elements(elements)
    {
        calculateOffsetsAndStride();
    }

    [[nodiscard]] inline auto getStride() const -> std::uint32_t { return _stride; }

    [[nodiscard]] inline auto getElements() const -> std::vector<BufferElement> const&
    {
        return _elements;
    }

    auto begin() -> std::vector<BufferElement>::iterator { return _elements.begin(); }

    auto end() -> std::vector<BufferElement>::iterator { return _elements.end(); }

    [[nodiscard]] auto begin() const -> std::vector<BufferElement>::const_iterator
    {
        return _elements.begin();
    }

    [[nodiscard]] auto end() const -> std::vector<BufferElement>::const_iterator
    {
        return _elements.end();
    }

private:
    void calculateOffsetsAndStride()
    {
        size_t offset = 0;
        _stride       = 0;
        for (auto& element : _elements) {
            element.Offset = offset;
            offset += element.Size;
            _stride += element.Size;
        }
    }

    std::vector<BufferElement> _elements;
    uint32_t _stride = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual auto bind() const -> void                                   = 0;
    virtual auto unbind() const -> void                                 = 0;
    [[nodiscard]] virtual auto getLayout() const -> BufferLayout const& = 0;
    virtual auto setLayout(BufferLayout const& layout) -> void          = 0;
    virtual auto
    uploadData(std::uint32_t offset, std::uint32_t size, void const* data) const -> void
        = 0;
};

struct IndexBufferSpecs
{
    std::span<std::uint32_t> indices = {};
    std::uint32_t count              = 0;
    bool isDynamic                   = false;
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const                                    = 0;
    virtual void unbind() const                                  = 0;
    [[nodiscard]] virtual auto getCount() const -> std::uint32_t = 0;
    virtual auto uploadData(std::uint32_t offset, std::span<std::uint32_t> data) const
        -> void
        = 0;
};

}  // namespace moci
