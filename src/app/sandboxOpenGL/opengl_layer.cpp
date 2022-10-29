#include "opengl_layer.hpp"

#include "assets.hpp"

#include "imgui.h"

namespace
{
glm::vec2 imGuiToGlmVec(ImVec2 const& input) { return {input.x, input.y}; }

}  // namespace
void OpenGLLayer::onAttach()
{
    auto const path = fmt::format("assets/shader/basic_{}.glsl", ShaderSuffix);
    auto samplers   = std::array {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    shader_         = moci::RenderFactory::makeShader(path);
    shader_->bind();
    shader_->setInts("u_Textures", (int)samplers.size(), samplers.data());

    auto const verticesSize = static_cast<std::uint32_t>(assets::quadVertices.size() * sizeof(float));
    auto layout             = moci::BufferLayout {
        {moci::ShaderDataType::Float3, "position"},      //
        {moci::ShaderDataType::Float4, "color"},         //
        {moci::ShaderDataType::Float2, "textureCoord"},  //
    };
    vbo_.reset(moci::RenderFactory::makeVertexBuffer(assets::quadVertices.data(), verticesSize));
    vbo_->setLayout(layout);
    vbo_->unbind();
    auto indices  = moci::Span<std::uint32_t> {assets::quadIndices.data(), assets::quadIndices.size()};
    auto specs    = moci::IndexBufferSpecs {};
    specs.indices = indices;
    specs.count   = static_cast<std::uint32_t>(indices.size());
    ibo_.reset(moci::RenderFactory::makeIndexBuffer(specs));
    ibo_->unbind();
    vao_ = moci::RenderFactory::makeVertexArray();
    vao_->addVertexBuffer(vbo_);
    vao_->setIndexBuffer(ibo_);
    vao_->unbind();

    texture_ = moci::RenderFactory::makeTexture2D("sandbox3D/assets/textures/skybox/back.jpg");

    auto fbSpec   = moci::FramebufferSpecs {};
    fbSpec.width  = 1920;
    fbSpec.height = 1080;
    framebuffer_  = moci::RenderFactory::makeFramebuffer(fbSpec);
}

void OpenGLLayer::onUpdate(moci::Timestep ts)
{
    moci::ignoreUnused(ts);

    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1.0F});
    moci::RenderCommand::clear();

    if (framebufferNeedsResize_)
    {
        auto const newWidth  = static_cast<std::uint32_t>(viewportSize_.x);
        auto const newHeight = static_cast<std::uint32_t>(viewportSize_.y);
        framebuffer_->resize(newWidth, newHeight);
        framebufferNeedsResize_ = false;
    }
    framebuffer_->bind();
    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1.0F});
    moci::RenderCommand::clear();

    vao_->bind();
    texture_->bind(0);
    moci::RenderCommand::drawIndexed(moci::RenderDrawMode::Triangles, vao_);
    framebuffer_->unbind();
}

void OpenGLLayer::onImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
    ImGui::Begin("Viewport");
    auto const viewportRegion = imGuiToGlmVec(ImGui::GetContentRegionAvail());
    if (viewportSize_ != viewportRegion)
    {
        viewportSize_           = viewportRegion;
        framebufferNeedsResize_ = true;
    }

    auto* const textureID = reinterpret_cast<void*>((size_t)framebuffer_->getColorAttachmentRendererId());
    ImGui::Image(textureID, {viewportSize_.x, viewportSize_.y}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}

void OpenGLLayer::onEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.dispatch<moci::WindowResizeEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleWindowResized));
    dispatcher.dispatch<moci::KeyPressedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleKeyPressed));
    dispatcher.dispatch<moci::MouseScrolledEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMouseScrolled));
    dispatcher.dispatch<moci::MouseMovedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMouseMoved));
    dispatcher.dispatch<moci::MouseButtonPressedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMousePressed));
    dispatcher.dispatch<moci::MouseButtonReleasedEvent>(MOCI_EVENT_METHOD(OpenGLLayer::handleMouseReleased));
}

bool OpenGLLayer::handleWindowResized(moci::WindowResizeEvent& e)
{
    width_  = static_cast<float>(e.getWidth());
    height_ = static_cast<float>(e.getHeight());
    return false;
}

bool OpenGLLayer::handleKeyPressed(moci::KeyPressedEvent& e)
{
    moci::ignoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMouseScrolled(moci::MouseScrolledEvent& e)
{
    moci::ignoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMousePressed(moci::MouseButtonPressedEvent& e)
{
    moci::ignoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMouseReleased(moci::MouseButtonReleasedEvent& e)
{
    moci::ignoreUnused(e);
    return true;
}

bool OpenGLLayer::handleMouseMoved(moci::MouseMovedEvent& e)
{
    moci::ignoreUnused(e);
    return true;
}
