#include "vertex_array.hpp"

#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{
static auto ShaderDataTypeToOpenGLESBaseType(ShaderDataType type) -> GLenum
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

void OpenGLESVertexArray::Bind() const
{
    m_VertexBuffer->Bind();
    setLayout();
    m_IndexBuffer->Bind();
}
void OpenGLESVertexArray::Unbind() const
{
    m_IndexBuffer->Unbind();
    m_VertexBuffer->Unbind();
}

void OpenGLESVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { m_VertexBuffer = vertexBuffer; }
void OpenGLESVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) { m_IndexBuffer = indexBuffer; }
void OpenGLESVertexArray::setLayout() const
{
    const auto& elements = m_VertexBuffer->GetLayout().GetElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(                               //
            i,                                               //
            element.GetComponentCount(),                     //
            ShaderDataTypeToOpenGLESBaseType(element.Type),  //
            element.Normalized ? GL_TRUE : GL_FALSE,         //
            m_VertexBuffer->GetLayout().GetStride(),         //
            (const void*)element.Offset                      //
        );
    }
}
}  // namespace moci