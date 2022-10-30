#include "vertex_array.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"
#include "moci/core/benchmark/profile.hpp"

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

    glCreateVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

void OpenGLVertexArray::bind() const { glBindVertexArray(m_RendererID); }

void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{

    MOCI_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(m_RendererID);
    vertexBuffer->bind();

    const auto& layout = vertexBuffer->getLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex, element.getComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.getStride(), (const void*)element.Offset);
        m_VertexBufferIndex++;
    }

    m_VertexBuffer = vertexBuffer;
    // m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{

    glBindVertexArray(m_RendererID);
    indexBuffer->bind();

    m_IndexBuffer = indexBuffer;
}

}  // namespace moci

#endif