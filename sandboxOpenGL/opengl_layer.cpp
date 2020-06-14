#include "opengl_layer.hpp"

void OpenGLLayer::OnAttach() { }

void OpenGLLayer::OnUpdate(moci::Timestep ts)
{
    moci::IgnoreUnused(ts);

    moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    moci::RenderCommand::Clear();
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
