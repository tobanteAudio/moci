#include "opengl_layer.hpp"

#include "assets.hpp"

void OpenGLLayer::OnAttach()
{
    auto const path = fmt::format("assets/shader/basic_{}.glsl", shaderSuffix);
    shader_         = moci::RenderFactory::MakeShader(path);
    shader_->Bind();

    auto const verticesSize = static_cast<std::uint32_t>(assets::QuadVertices.size() * sizeof(float));
    auto layout             = moci::BufferLayout {
        {moci::ShaderDataType::Float3, "position"},  //
        {moci::ShaderDataType::Float4, "color"},     //
    };
    vbo_.reset(moci::RenderFactory::MakeVertexBuffer(assets::QuadVertices.data(), verticesSize));
    vbo_->SetLayout(layout);
    vbo_->Unbind();
    ibo_.reset(moci::RenderFactory::MakeIndexBuffer(assets::QuadIndices.data(), assets::QuadIndices.size()));
    ibo_->Unbind();
    vao_ = moci::RenderFactory::MakeVertexArray();
    vao_->AddVertexBuffer(vbo_);
    vao_->SetIndexBuffer(ibo_);
    vao_->Unbind();
}

void OpenGLLayer::OnUpdate(moci::Timestep ts)
{
    moci::IgnoreUnused(ts);

    moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    moci::RenderCommand::Clear();

    vao_->Bind();
    moci::RenderCommand::DrawIndexed(vao_);
}

void OpenGLLayer::OnImGuiRender() { }

void OpenGLLayer::OnEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleWindowResized));
    dispatcher.Dispatch<moci::KeyPressedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleKeyPressed));
    dispatcher.Dispatch<moci::MouseScrolledEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMouseScrolled));
    dispatcher.Dispatch<moci::MouseMovedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMouseMoved));
    dispatcher.Dispatch<moci::MouseButtonPressedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMousePressed));
    dispatcher.Dispatch<moci::MouseButtonReleasedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMouseReleased));
}

bool OpenGLLayer::handleWindowResized(moci::WindowResizeEvent& e)
{
    width_  = static_cast<float>(e.GetWidth());
    height_ = static_cast<float>(e.GetHeight());
    return false;
}

bool OpenGLLayer::handleKeyPressed(moci::KeyPressedEvent& e)
{
    moci::IgnoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMouseScrolled(moci::MouseScrolledEvent& e)
{
    moci::IgnoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMousePressed(moci::MouseButtonPressedEvent& e)
{
    moci::IgnoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMouseReleased(moci::MouseButtonReleasedEvent& e)
{
    moci::IgnoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMouseMoved(moci::MouseMovedEvent& e)
{
    moci::IgnoreUnused(e);
    return true;
}
