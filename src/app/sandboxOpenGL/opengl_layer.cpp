#include "opengl_layer.hpp"

#include "assets.hpp"

#include "imgui.h"

namespace {
auto imGuiToGlmVec(ImVec2 const& input) -> glm::vec2 { return {input.x, input.y}; }

}  // namespace

void OpenGLLayer::onAttach()
{
    auto const path = fmt::format("assets/shader/basic_{}.glsl", ShaderSuffix);
    auto samplers   = std::array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    _shader         = moci::RenderFactory::makeShader(path);
    _shader->bind();
    _shader->setInts("u_Textures", (int)samplers.size(), samplers.data());

    auto const verticesSize
        = static_cast<std::uint32_t>(assets::quadVertices.size() * sizeof(float));
    auto layout = moci::BufferLayout{
        {moci::ShaderDataType::Float3, "position"    }, //
        {moci::ShaderDataType::Float4, "color"       }, //
        {moci::ShaderDataType::Float2, "textureCoord"}, //
    };
    _vbo.reset(
        moci::RenderFactory::makeVertexBuffer(assets::quadVertices.data(), verticesSize)
    );
    _vbo->setLayout(layout);
    _vbo->unbind();
    auto indices
        = std::span<std::uint32_t>{assets::quadIndices.data(), assets::quadIndices.size()};
    auto specs    = moci::IndexBufferSpecs{};
    specs.indices = indices;
    specs.count   = static_cast<std::uint32_t>(indices.size());
    _ibo.reset(moci::RenderFactory::makeIndexBuffer(specs));
    _ibo->unbind();
    _vao = moci::RenderFactory::makeVertexArray();
    _vao->addVertexBuffer(_vbo);
    _vao->setIndexBuffer(_ibo);
    _vao->unbind();

    _texture = moci::RenderFactory::makeTexture2D(
        "src/app/sandbox3D/assets/textures/skybox/back.jpg"
    );

    auto fbSpec   = moci::FramebufferSpecs{};
    fbSpec.width  = 1920;
    fbSpec.height = 1080;
    _framebuffer  = moci::RenderFactory::makeFramebuffer(fbSpec);
}

void OpenGLLayer::onUpdate(moci::Timestep ts)
{
    moci::ignoreUnused(ts);

    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1.0F});
    moci::RenderCommand::clear();

    if (_framebufferNeedsResize) {
        auto const newWidth  = static_cast<std::uint32_t>(_viewportSize.x);
        auto const newHeight = static_cast<std::uint32_t>(_viewportSize.y);
        _framebuffer->resize(newWidth, newHeight);
        _framebufferNeedsResize = false;
    }
    _framebuffer->bind();
    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1.0F});
    moci::RenderCommand::clear();

    _vao->bind();
    _texture->bind(0);
    moci::RenderCommand::drawIndexed(moci::RenderDrawMode::Triangles, _vao);
    _framebuffer->unbind();
}

void OpenGLLayer::onImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    auto const viewportRegion = imGuiToGlmVec(ImGui::GetContentRegionAvail());
    if (_viewportSize != viewportRegion) {
        _viewportSize           = viewportRegion;
        _framebufferNeedsResize = true;
    }

    auto* const textureID
        = reinterpret_cast<void*>((size_t)_framebuffer->getColorAttachmentRendererId());
    ImGui::Image(textureID, {_viewportSize.x, _viewportSize.y}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}

void OpenGLLayer::onEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.dispatch<moci::WindowResizeEvent>(
        MOCI_EVENT_METHOD(OpenGLLayer::handleWindowResized)
    );
    dispatcher.dispatch<moci::KeyPressedEvent>(
        MOCI_EVENT_METHOD(OpenGLLayer::handleKeyPressed)
    );
    dispatcher.dispatch<moci::MouseScrolledEvent>(
        MOCI_EVENT_METHOD(OpenGLLayer::handleMouseScrolled)
    );
    dispatcher.dispatch<moci::MouseMovedEvent>(
        MOCI_EVENT_METHOD(OpenGLLayer::handleMouseMoved)
    );
    dispatcher.dispatch<moci::MouseButtonPressedEvent>(
        MOCI_EVENT_METHOD(OpenGLLayer::handleMousePressed)
    );
    dispatcher.dispatch<moci::MouseButtonReleasedEvent>(
        MOCI_EVENT_METHOD(OpenGLLayer::handleMouseReleased)
    );
}

auto OpenGLLayer::handleWindowResized(moci::WindowResizeEvent& e) -> bool
{
    _width  = static_cast<float>(e.getWidth());
    _height = static_cast<float>(e.getHeight());
    return false;
}

auto OpenGLLayer::handleKeyPressed(moci::KeyPressedEvent& e) -> bool
{
    moci::ignoreUnused(e);
    return true;
}

auto OpenGLLayer::handleMouseScrolled(moci::MouseScrolledEvent& e) -> bool
{
    moci::ignoreUnused(e);
    return true;
}

auto OpenGLLayer::handleMousePressed(moci::MouseButtonPressedEvent& e) -> bool
{
    moci::ignoreUnused(e);
    return true;
}

auto OpenGLLayer::handleMouseReleased(moci::MouseButtonReleasedEvent& e) -> bool
{
    moci::ignoreUnused(e);
    return true;
}

auto OpenGLLayer::handleMouseMoved(moci::MouseMovedEvent& e) -> bool
{
    moci::ignoreUnused(e);
    return true;
}
