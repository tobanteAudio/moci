#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

struct Vertex
{
    glm::vec3 position {};
    glm::vec4 color {};
};

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static Vertex CubeVertices[] = {
    {{-1.0f, -1.0f, -1.0f}},  // triangle 1 : begin
    {{-1.0f, -1.0f, 1.0f}},   //
    {{-1.0f, 1.0f, 1.0f}},    // triangle 1 : end
    {{1.0f, 1.0f, -1.0f}},    // triangle 2 : begin
    {{-1.0f, -1.0f, -1.0f}},  //
    {{-1.0f, 1.0f, -1.0f}},   // triangle 2 : end
    {{1.0f, -1.0f, 1.0f}},    //
    {{-1.0f, -1.0f, -1.0f}},  //
    {{1.0f, -1.0f, -1.0f}},   //
    {{1.0f, 1.0f, -1.0f}},    //
    {{1.0f, -1.0f, -1.0f}},   //
    {{-1.0f, -1.0f, -1.0f}},  //
    {{-1.0f, -1.0f, -1.0f}},  //
    {{-1.0f, 1.0f, 1.0f}},    //
    {{-1.0f, 1.0f, -1.0f}},   //
    {{1.0f, -1.0f, 1.0f}},    //
    {{-1.0f, -1.0f, 1.0f}},   //
    {{-1.0f, -1.0f, -1.0f}},  //
    {{-1.0f, 1.0f, 1.0f}},    //
    {{-1.0f, -1.0f, 1.0f}},   //
    {{1.0f, -1.0f, 1.0f}},    //
    {{1.0f, 1.0f, 1.0f}},     //
    {{1.0f, -1.0f, -1.0f}},   //
    {{1.0f, 1.0f, -1.0f}},    //
    {{1.0f, -1.0f, -1.0f}},   //
    {{1.0f, 1.0f, 1.0f}},     //
    {{1.0f, -1.0f, 1.0f}},    //
    {{1.0f, 1.0f, 1.0f}},     //
    {{1.0f, 1.0f, -1.0f}},    //
    {{-1.0f, 1.0f, -1.0f}},   //
    {{1.0f, 1.0f, 1.0f}},     //
    {{-1.0f, 1.0f, -1.0f}},   //
    {{-1.0f, 1.0f, 1.0f}},    //
    {{1.0f, 1.0f, 1.0f}},     //
    {{-1.0f, 1.0f, 1.0f}},    //
    {{1.0f, -1.0f, 1.0f}},    //
};

class DemoLayer : public moci::Layer
{
public:
    DemoLayer()           = default;
    ~DemoLayer() override = default;

    void OnAttach() override
    {

        auto col = 0.0f;
        for (auto& vertex : CubeVertices)
        {
            vertex.color = {col * 2.0f, col * 5.0f, 0.0f, 1.0f};
            col += 0.01;
        }

        shader_ = moci::Shader::Create("sandbox3D/shader3D.glsl");
        shader_->Bind();

        moci::BufferLayout layout = {
            {moci::ShaderDataType::Float3, "position"},  //
            {moci::ShaderDataType::Float4, "color"},     //
        };

        std::vector<Vertex> cube {};
        model.Parse();
        for (auto const& vertex : model.GetVertexData())
        {
            cube.push_back(Vertex {vertex.position, moci::Colors::Blue.GetData()});
        }

        MOCI_INFO("Num vertices: {}", cube.size());
        vbo_.reset(
            moci::VertexBuffer::Create(reinterpret_cast<float*>(cube.data()), cube.size() * sizeof(Vertex), false));
        // vbo_.reset(moci::VertexBuffer::Create(reinterpret_cast<float*>(CubeVertices), sizeof(CubeVertices), false));
        vbo_->SetLayout(layout);
        vbo_->Unbind();
        ibo_.reset(moci::IndexBuffer::Create(nullptr, 1, true));
        ibo_->Unbind();
        vao_ = moci::VertexArray::Create();
        vao_->AddVertexBuffer(vbo_);
        vao_->SetIndexBuffer(ibo_);
        vao_->Unbind();
    }

    void OnUpdate(moci::Timestep ts) override
    {
        moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        moci::RenderCommand::Clear();

        // Camera matrix
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), width_ / height_, 0.1f, 100.0f);
        glm::mat4 View       = glm::lookAt(cameraPos_,          // Camera is at (4,3,3), in World Space
                                     glm::vec3(0, 0, 0),  // and looks at the origin
                                     glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
        );

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 proj  = Projection * View * Model;  // Remember, matrix multiplication is the other way around
        shader_->SetMat4("u_MVP", proj);

        vao_->Bind();
        moci::RenderCommand::DrawArrays(moci::RendererAPI::DrawMode::Triangles, 0, model.GetVertexData().size());
    }

    void OnEvent(moci::Event& e) override
    {
        moci::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_BIND_EVENT_FN(DemoLayer::OnWindowResized));
        dispatcher.Dispatch<moci::KeyPressedEvent>(MOCI_BIND_EVENT_FN(DemoLayer::OnKeyPressed));
    }

    bool OnWindowResized(moci::WindowResizeEvent& e)
    {
        width_  = static_cast<float>(e.GetWidth());
        height_ = static_cast<float>(e.GetHeight());
        return false;
    }

    bool OnKeyPressed(moci::KeyPressedEvent& e)
    {
        auto const moveAmount = 0.25f;
        if (e.GetKeyCode() == MOCI_KEY_LEFT)
        {
            cameraPos_.x = cameraPos_.x - moveAmount;
        }
        if (e.GetKeyCode() == MOCI_KEY_RIGHT)
        {
            cameraPos_.x = cameraPos_.x + moveAmount;
        }
        if (e.GetKeyCode() == MOCI_KEY_DOWN)
        {
            cameraPos_.y = cameraPos_.y - moveAmount;
        }
        if (e.GetKeyCode() == MOCI_KEY_UP)
        {
            cameraPos_.y = cameraPos_.y + moveAmount;
        }
        return true;
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Number: %d", 1);
        ImGui::End();
    }

public:
    float width_  = 1280.0f;
    float height_ = 1024.0f;

    glm::vec3 cameraPos_ {80.0f, 80.0f, 60.0f};

    moci::OBJFile model {"sandbox3D/red_pepper.obj"};

    std::shared_ptr<moci::Shader> shader_;
    std::shared_ptr<moci::VertexBuffer> vbo_;
    std::shared_ptr<moci::IndexBuffer> ibo_;
    std::shared_ptr<moci::VertexArray> vao_;
};

class Sandbox : public moci::Application
{
public:
    Sandbox()
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox-3d", "moci-sandbox-3d.json");
        PushLayer(new DemoLayer());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

auto moci::CreateApplication() -> moci::Application* { return new Sandbox(); }