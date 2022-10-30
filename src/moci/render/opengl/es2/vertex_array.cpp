#include "vertex_array.hpp"

#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{
static auto shaderDataTypeToOpenGlesBaseType(ShaderDataType type) -> GLenum
{
    switch (type)
    {
        case moci::ShaderDataType::Float: return GL_FLOAT;
        case moci::ShaderDataType::Float2: return GL_FLOAT;
        case moci::ShaderDataType::Float3: return GL_FLOAT;
        case moci::ShaderDataType::Float4: return GL_FLOAT;
        case moci::ShaderDataType::Mat3: return GL_FLOAT;
        case moci::ShaderDataType::Mat4: return GL_FLOAT;
        case moci::ShaderDataType::Int: return GL_INT;
        case moci::ShaderDataType::Int2: return GL_INT;
        case moci::ShaderDataType::Int3: return GL_INT;
        case moci::ShaderDataType::Int4: return GL_INT;
        case moci::ShaderDataType::Bool: return GL_BOOL;
        case moci::ShaderDataType::None: MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!"); return 0;
    }

    MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

OpenGLESVertexArray::OpenGLESVertexArray()  = default;
OpenGLESVertexArray::~OpenGLESVertexArray() = default;

void OpenGLESVertexArray::bind() const
{
    _vertexBuffer->bind();
    setLayout();
    _indexBuffer->bind();
}
void OpenGLESVertexArray::unbind() const
{
    _indexBuffer->unbind();
    _vertexBuffer->unbind();
}

void OpenGLESVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { _vertexBuffer = vertexBuffer; }
void OpenGLESVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) { _indexBuffer = indexBuffer; }
void OpenGLESVertexArray::setLayout() const
{
    const auto& elements = _vertexBuffer->getLayout().getElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(                               //
            i,                                               //
            element.getComponentCount(),                     //
            shaderDataTypeToOpenGlesBaseType(element.Type),  //
            element.Normalized ? GL_TRUE : GL_FALSE,         //
            _vertexBuffer->getLayout().getStride(),          //
            (const void*)element.Offset                      //
        );
    }
}
}  // namespace moci