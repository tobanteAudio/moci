#include "vertex_array.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"
#include <moci/core/benchmark/profile.hpp>

namespace moci
{

static auto ShaderDataTypeToOpenGLBaseType(ShaderDataType type) -> GLenum
{
    switch (type)
    {
        case ShaderDataType::Float: return GL_FLOAT;
        case ShaderDataType::Float2: return GL_FLOAT;
        case ShaderDataType::Float3: return GL_FLOAT;
        case ShaderDataType::Float4: return GL_FLOAT;
        case ShaderDataType::Mat3: return GL_FLOAT;
        case ShaderDataType::Mat4: return GL_FLOAT;
        case ShaderDataType::Int: return GL_INT;
        case ShaderDataType::Int2: return GL_INT;
        case ShaderDataType::Int3: return GL_INT;
        case ShaderDataType::Int4: return GL_INT;
        case ShaderDataType::Bool: return GL_BOOL;
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    MOCI_PROFILE_FUNCTION();

    glCreateVertexArrays(1, &_rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &_rendererID); }

void OpenGLVertexArray::bind() const { glBindVertexArray(_rendererID); }

void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{

    MOCI_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(_rendererID);
    vertexBuffer->bind();

    const auto& layout = vertexBuffer->getLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(_vertexBufferIndex);
        glVertexAttribPointer(_vertexBufferIndex, element.getComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.getStride(), (const void*)element.Offset);
        _vertexBufferIndex++;
    }

    _vertexBuffer = vertexBuffer;
    // _vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{

    glBindVertexArray(_rendererID);
    indexBuffer->bind();

    _indexBuffer = indexBuffer;
}

}  // namespace moci

#endif