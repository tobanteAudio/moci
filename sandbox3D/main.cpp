#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "imgui.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "glm/gtx/quaternion.hpp"

#include "assimp/DefaultLogger.hpp"
#include "assimp/Importer.hpp"
#include "assimp/LogStream.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

struct Vertex
{
    glm::vec3 position {};
    glm::vec3 normal {};
    glm::vec4 color {};
    glm::vec2 texCoord {};
};

struct Index
{
    uint32_t V1, V2, V3;
};

class Submesh
{
public:
    uint32_t BaseVertex;
    uint32_t BaseIndex;
    uint32_t MaterialIndex;
    uint32_t IndexCount;

    glm::mat4 Transform;
};

static constexpr uint32_t s_MeshImportFlags
    =                        //
                             // aiProcess_CalcTangentSpace |           // Create binormals/tangents just in case
    aiProcess_Triangulate |  // Make sure we're triangles
    aiProcess_SortByPType |  // Split meshes by primitive type
    aiProcess_GenNormals |   // Make sure we have legit normals
    // aiProcess_GenUVCoords |                // Convert UVs if required
    aiProcess_OptimizeMeshes |        // Batch draws where possible
    aiProcess_ValidateDataStructure;  // Validation

struct LogStream : public Assimp::LogStream
{
    static void Initialize()
    {
        if (Assimp::DefaultLogger::isNullLogger())
        {
            Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
            Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
        }
    }

    void write(const char* message) override { MOCI_CORE_ERROR("Assimp error: {0}", message); }
};

static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
    glm::mat4 to;
    // the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    // clang-format off
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    // clang-format on
    return to;
}

class Mesh
{
public:
    Mesh(const std::string& filename) : m_FilePath(filename)
    {
        LogStream::Initialize();

        MOCI_CORE_INFO("Loading mesh: {0}", filename.c_str());

        m_Importer = std::make_unique<Assimp::Importer>();

        const aiScene* scene = m_Importer->ReadFile(filename, s_MeshImportFlags);
        if (!scene || !scene->HasMeshes()) MOCI_CORE_ERROR("Failed to load mesh file: {0}", filename);

        m_IsAnimated = scene->mAnimations != nullptr;
        // m_MeshShader = m_IsAnimated ? Renderer::GetShaderLibrary()->Get("HazelPBR_Anim")
        //                             : Renderer::GetShaderLibrary()->Get("HazelPBR_Static");
        // m_Material.reset(new moci::Material(m_MeshShader));
        m_InverseTransform = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));

        uint32_t vertexCount = 0;
        uint32_t indexCount  = 0;

        m_Submeshes.reserve(scene->mNumMeshes);
        for (size_t m = 0; m < scene->mNumMeshes; m++)
        {
            aiMesh* mesh = scene->mMeshes[m];

            Submesh submesh;
            submesh.BaseVertex    = vertexCount;
            submesh.BaseIndex     = indexCount;
            submesh.MaterialIndex = mesh->mMaterialIndex;
            submesh.IndexCount    = mesh->mNumFaces * 3;
            m_Submeshes.push_back(submesh);

            vertexCount += mesh->mNumVertices;
            indexCount += submesh.IndexCount;

            MOCI_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
            MOCI_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

            // Vertices
            if (m_IsAnimated)
            {
                // for (size_t i = 0; i < mesh->mNumVertices; i++)
                // {
                //     AnimatedVertex vertex;
                //     vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                //     vertex.Normal   = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

                //     if (mesh->HasTangentsAndBitangents())
                //     {
                //         vertex.Tangent  = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                //         vertex.Binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
                //     }

                //     if (mesh->HasTextureCoords(0))
                //         vertex.Texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};

                //     m_AnimatedVertices.push_back(vertex);
                // }
            }
            else
            {
                for (size_t i = 0; i < mesh->mNumVertices; i++)
                {
                    Vertex vertex;
                    vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                    vertex.normal   = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
                    vertex.color    = moci::Colors::Blue.GetData();

                    // if (mesh->HasTangentsAndBitangents())
                    // {
                    //     vertex.Tangent  = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                    //     vertex.Binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
                    // }

                    if (mesh->HasTextureCoords(0))
                        vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};

                    m_StaticVertices.push_back(vertex);
                }
            }

            // Indices
            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                MOCI_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
                m_Indices.push_back(
                    {mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]});
            }
        }

        // MOCI_CORE_TRACE("NODES:");
        // MOCI_CORE_TRACE("-----------------------------");
        // TraverseNodes(scene->mRootNode);
        // MOCI_CORE_TRACE("-----------------------------");

        // // Bones
        // if (m_IsAnimated)
        // {
        //     for (size_t m = 0; m < scene->mNumMeshes; m++)
        //     {
        //         aiMesh* mesh     = scene->mMeshes[m];
        //         Submesh& submesh = m_Submeshes[m];

        //         for (size_t i = 0; i < mesh->mNumBones; i++)
        //         {
        //             aiBone* bone = mesh->mBones[i];
        //             std::string boneName(bone->mName.data);
        //             int boneIndex = 0;

        //             if (m_BoneMapping.find(boneName) == m_BoneMapping.end())
        //             {
        //                 // Allocate an index for a new bone
        //                 boneIndex = m_BoneCount;
        //                 m_BoneCount++;
        //                 BoneInfo bi;
        //                 m_BoneInfo.push_back(bi);
        //                 m_BoneInfo[boneIndex].BoneOffset = aiMatrix4x4ToGlm(bone->mOffsetMatrix);
        //                 m_BoneMapping[boneName]          = boneIndex;
        //             }
        //             else
        //             {
        //                 MOCI_CORE_TRACE("Found existing bone in map");
        //                 boneIndex = m_BoneMapping[boneName];
        //             }

        //             for (size_t j = 0; j < bone->mNumWeights; j++)
        //             {
        //                 int VertexID = submesh.BaseVertex + bone->mWeights[j].mVertexId;
        //                 float Weight = bone->mWeights[j].mWeight;
        //                 m_AnimatedVertices[VertexID].AddBoneData(boneIndex, Weight);
        //             }
        //         }
        //     }
        // }

        // m_VertexArray = VertexArray::Create();
        // if (m_IsAnimated)
        // {
        //     auto vb
        //         = VertexBuffer::Create(m_AnimatedVertices.data(), m_AnimatedVertices.size() *
        //         sizeof(AnimatedVertex));
        //     vb->SetLayout({
        //         {ShaderDataType::Float3, "a_Position"},
        //         {ShaderDataType::Float3, "a_Normal"},
        //         {ShaderDataType::Float3, "a_Tangent"},
        //         {ShaderDataType::Float3, "a_Binormal"},
        //         {ShaderDataType::Float2, "a_TexCoord"},
        //         {ShaderDataType::Int4, "a_BoneIDs"},
        //         {ShaderDataType::Float4, "a_BoneWeights"},
        //     });
        //     m_VertexArray->AddVertexBuffer(vb);
        // }
        // else
        // {
        //     auto vb = VertexBuffer::Create(m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
        //     vb->SetLayout({
        //         {ShaderDataType::Float3, "a_Position"},
        //         {ShaderDataType::Float3, "a_Normal"},
        //         {ShaderDataType::Float3, "a_Tangent"},
        //         {ShaderDataType::Float3, "a_Binormal"},
        //         {ShaderDataType::Float2, "a_TexCoord"},
        //     });
        //     m_VertexArray->AddVertexBuffer(vb);
        // }

        // auto ib = IndexBuffer::Create(m_Indices.data(), m_Indices.size() * sizeof(Index));
        // m_VertexArray->SetIndexBuffer(ib);
        // m_Scene = scene;
    }

    gsl::span<Vertex const> GetVertices() const noexcept { return m_StaticVertices; }

private:
    std::string m_FilePath {};
    std::unique_ptr<Assimp::Importer> m_Importer {};
    glm::mat4 m_InverseTransform {};
    std::vector<Submesh> m_Submeshes {};
    std::vector<Vertex> m_StaticVertices {};
    bool m_IsAnimated = false;
    std::vector<Index> m_Indices {};
};

class DemoLayer : public moci::Layer
{
public:
    DemoLayer()           = default;
    ~DemoLayer() override = default;

    void OnAttach() override
    {
        MOCI_PROFILE_FUNCTION();
        {
            MOCI_PROFILE_SCOPE("OnAttach::GPUQuery");
            MOCI_INFO("Max vertex attributes: {}", moci::RenderCommand::MaxVertexAttributes());
            MOCI_INFO("Max texture size: {}", moci::RenderCommand::MaxTextureSize());
            MOCI_INFO("Max texture units: {}", moci::RenderCommand::MaxTextureUnits());
            MOCI_INFO("Max uniform vectors: {}", moci::RenderCommand::MaxUniformVectors());
        }

        auto const numVertices = (mesh_.GetVertices().size() * 11)  //
                                 + floor_.GetVertices().size()      //
                                 + lightMesh_.GetVertices().size();

        vertices_.reserve(numVertices);
        {
            MOCI_PROFILE_SCOPE("OnAttach::Shader");
#if defined(MOCI_API_OPENGL_LEGACY)
            shader_ = moci::Shader::Create("sandbox3D/assets/shader/es2_general.glsl");
#else
            shader_        = moci::Shader::Create("sandbox3D/assets/shader/gl4_general.glsl");
#endif
            shader_->Bind();
        }

        {
            MOCI_PROFILE_SCOPE("Translate");
            for (auto const& vertex : mesh_.GetVertices())
            {
                auto const model       = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
                auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {modelScale_, modelScale_, modelScale_});
                auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
                vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
            }

            {
                MOCI_PROFILE_SCOPE("Translate Small Teapots");
                for (auto x = 1; x <= 10; x++)
                {
                    auto const modelScale = modelScale_ / 2.0f;
                    auto const scale      = glm::scale(glm::mat4(1.0f), {modelScale, modelScale, modelScale});
                    auto const model      = glm::translate(glm::mat4(1.0f), glm::vec3((3.0f * x), 5.0f, 1.0f));
                    for (auto const& vertex : mesh_.GetVertices())
                    {
                        auto const position = model * scale * glm::vec4(vertex.position, 1.0f);
                        vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
                    }
                }
            }

            for (auto const& vertex : floor_.GetVertices())
            {
                auto const model       = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
                auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {5.0f, 5.0f, 5.0f});
                auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
                vertices_.push_back({glm::vec3(position), vertex.normal, {1.0f, 1.0f, 0.5f, 1.0f}, vertex.texCoord});
            }

            numVertices_ = vertices_.size();
        }

        // Mesh buffer
        {
            MOCI_PROFILE_SCOPE("OnAttach::MeshBuffer");
            moci::BufferLayout layout = {
                {moci::ShaderDataType::Float3, "a_Position"},   //
                {moci::ShaderDataType::Float3, "a_Normal"},     //
                {moci::ShaderDataType::Float4, "a_Color"},      //
                {moci::ShaderDataType::Float2, "a_TexCoords"},  //
            };
            auto* data      = reinterpret_cast<float*>(vertices_.data());
            auto const size = static_cast<std::uint32_t>(vertices_.size() * sizeof(Vertex));
            vbo_.reset(moci::VertexBuffer::Create(data, size, false));
            vbo_->SetLayout(layout);
            vbo_->Unbind();
            ibo_.reset(moci::IndexBuffer::Create(nullptr, 1, true));
            ibo_->Unbind();
            vao_ = moci::VertexArray::Create();
            vao_->AddVertexBuffer(vbo_);
            vao_->SetIndexBuffer(ibo_);
            vao_->Unbind();
        }
        {
            // Light buffer
            MOCI_PROFILE_SCOPE("OnAttach::LightBuffer");
#if defined(MOCI_API_OPENGL_LEGACY)
            light.shader = moci::Shader::Create("sandbox3D/assets/shader/es2_light_source.glsl");
#else
            light.shader   = moci::Shader::Create("sandbox3D/assets/shader/gl4_light_source.glsl");
#endif
            light.shader->Bind();
            moci::BufferLayout lightLayout = {
                {moci::ShaderDataType::Float3, "a_Position"},  //
                {moci::ShaderDataType::Float4, "a_Color"},     //
            };

            auto const size = static_cast<uint32_t>(lightMesh_.GetVertices().size() * sizeof(Light::Vertex));
            light.vbo.reset(moci::VertexBuffer::Create(nullptr, size, true));
            light.vbo->SetLayout(lightLayout);
            light.vbo->Unbind();
            light.ibo.reset(moci::IndexBuffer::Create(nullptr, 1, true));
            light.ibo->Unbind();
            light.vao = moci::VertexArray::Create();
            light.vao->AddVertexBuffer(light.vbo);
            light.vao->SetIndexBuffer(light.ibo);
            light.vao->Unbind();
        }

        {
            MOCI_PROFILE_SCOPE("OnAttach::Textures");
            textureSolid_  = moci::Texture2D::Create("sandbox3D/assets/textures/white_10x10.png");
            textureColors_ = moci::Texture2D::Create("sandbox3D/assets/textures/4color.png");
#if defined(MOCI_API_OPENGL_ES)
            textureColors_ = moci::Texture2D::Create("sandbox3D/assets/textures/cerberus_A_1024x1024.png");
#else
            textureColors_ = moci::Texture2D::Create("sandbox3D/assets/textures/cerberus_A_4096x4096.png");
#endif
        }

        fpsHistory_.reserve(10'000);
    }

    void OnUpdate(moci::Timestep ts) override
    {
        MOCI_PROFILE_FUNCTION();
        {
            MOCI_PROFILE_SCOPE("OnUpdate::Clear");
            lastTimestep_          = ts.GetMilliseconds();
            drawStats_.numVertices = 0;
            moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            moci::RenderCommand::Clear();
        }

        // Camera matrix
        glm::mat4 const projection = glm::perspective(glm::radians(cameraFOV_), width_ / height_, 0.1f, 100.0f);
        glm::mat4 const view       = glm::lookAt(  //
            cameraPos_,                      // Camera is at (x,y,z), in World Space
            cameraLookAt_,                   // and looks at the origin
            glm::vec3(0, 1, 0)               // Head is up (set to 0,-1,0 to look upside-down)
        );

        {
            MOCI_PROFILE_SCOPE("OnUpdate::Uniforms Mesh");
            shader_->Bind();
            shader_->SetMat4("u_View", view);
            shader_->SetMat4("u_Projection", projection);
            shader_->SetFloat("u_Ambient", ambientLight_);
            shader_->SetFloat3("u_LightPos", light.position);
            shader_->SetFloat3("u_LightColor", glm::vec3(light.color));
            shader_->SetFloat3("u_ViewPos", cameraPos_);
        }

        {
            MOCI_PROFILE_SCOPE("OnUpdate::DrawArrays Mesh");
            vao_->Bind();
            textureColors_->Bind(0);
            moci::RenderCommand::DrawArrays(moci::RendererAPI::DrawMode::Triangles, 0,
                                            static_cast<std::uint32_t>(vertices_.size()));
            drawStats_.numVertices += static_cast<std::uint32_t>(vertices_.size());
            textureColors_->Unbind();
        }

        {
            MOCI_PROFILE_SCOPE("OnUpdate::Translate Light");
            auto const model       = glm::translate(glm::mat4(1.0f), light.position);
            auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {light.scale, light.scale, light.scale});
            auto const color       = light.color;
            for (auto const& vertex : lightMesh_.GetVertices())
            {
                auto const position = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
                light.vertices.push_back({glm::vec3(position), color});
            }
        }

        {
            MOCI_PROFILE_SCOPE("OnUpdate::Uniforms Light");
            light.shader->Bind();
            light.shader->SetMat4("u_View", view);
            light.shader->SetMat4("u_Projection", projection);
        }

        {
            MOCI_PROFILE_SCOPE("OnUpdate::DrawArrays Light");
            light.vao->Bind();
            light.vbo->UploadData(0, light.vertices.size() * sizeof(Light::Vertex), light.vertices.data());
            moci::RenderCommand::DrawArrays(moci::RendererAPI::DrawMode::Triangles, 0, light.vertices.size());
            drawStats_.numVertices += light.vertices.size();
            light.vertices.clear();
        }
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
        MOCI_PROFILE_FUNCTION();
        if (drawStats_.resetCounter >= DrawStats::ResetRate)
        {
            drawStats_.minFPS       = 9999.0f;
            drawStats_.maxFPS       = 0.0f;
            drawStats_.resetCounter = 0;
        }
        drawStats_.resetCounter += 1;

        auto const fps = ImGui::GetIO().Framerate;
        if (fps < drawStats_.minFPS)
        {
            drawStats_.minFPS = fps;
        }
        if (fps > drawStats_.maxFPS)
        {
            drawStats_.maxFPS = fps;
        }

        auto const fpsStr    = fmt::format("FPS: {0:0.1f}", fps);
        auto const minFPSStr = fmt::format("Min: {0:0.1f}", drawStats_.minFPS);
        auto const maxFPSStr = fmt::format("Max: {0:0.1f}", drawStats_.maxFPS);
        auto const ts        = fmt::format("Timestep: {0:0.1f}", lastTimestep_);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Debug"))
            {
                ImGui::Checkbox("Sandbox3D", &imguiWindow_);
                ImGui::Checkbox("Imgui Demo", &imguiDemo_);
                ImGui::EndMenu();
            }

            ImGui::TextUnformatted(fpsStr.c_str());
            ImGui::TextUnformatted(minFPSStr.c_str());
            ImGui::TextUnformatted(maxFPSStr.c_str());
            ImGui::TextUnformatted(ts.c_str());
            ImGui::EndMenuBar();
        }

        if (imguiWindow_)
        {
            ImGui::Begin("Sandbox 3D", &imguiWindow_);

            if (ImGui::CollapsingHeader("Camera"))
            {
                ImGui::SliderFloat3("Position", glm::value_ptr(cameraPos_), -100.0f, 100.0f);
                ImGui::SliderFloat3("Look At", glm::value_ptr(cameraLookAt_), -10.0f, 10.0f);
                ImGui::SliderFloat("FOV", &cameraFOV_, 5.0f, 85.0f);
            }

            if (ImGui::CollapsingHeader("Light"))
            {

                ImGui::SliderFloat("Ambient", &ambientLight_, 0.0f, 0.4f);
                ImGui::SliderFloat3("Light Position", glm::value_ptr(light.position), -20.0f, 20.0f);
                ImGui::ColorEdit4("Light Color", glm::value_ptr(light.color), 0);
                ImGui::SliderFloat("Light Scale", &light.scale, 0.1f, 1.0f);
            }

            if (ImGui::CollapsingHeader("Model"))
            {
                ImGui::SliderFloat("Scale", &modelScale_, 0.01f, 20.0f);
            }

            if (ImGui::CollapsingHeader("Stats"))
            {
                MOCI_PROFILE_SCOPE("ImGui::Stats");
                fpsHistory_.push_back(fps);
                auto const vertices  = fmt::format("{} Vertices", drawStats_.numVertices);
                auto const triangles = fmt::format("{} Triangles", drawStats_.numVertices / 3);
                auto const mb        = drawStats_.numVertices * sizeof(Vertex) / 1'000'000.0f;
                auto const megabyte  = fmt::format("{0:0.1f} Mbytes", mb);
                ImGui::TextUnformatted(fpsStr.c_str());
                ImGui::TextUnformatted(minFPSStr.c_str());
                ImGui::TextUnformatted(maxFPSStr.c_str());
                ImGui::TextUnformatted(vertices.c_str());
                ImGui::TextUnformatted(triangles.c_str());
                ImGui::TextUnformatted(megabyte.c_str());
                auto const lightStr = fmt::format("Light vertices: {}", lightMesh_.GetVertices().size());
                ImGui::TextUnformatted(lightStr.c_str());
                auto const size = static_cast<int>(fpsHistory_.size());
                ImGui::PlotLines("FPS", fpsHistory_.data(), size, 0, "", 50.0f, 75.0f, ImVec2(0, 80));
            }
            ImGui::End();
        }

        // ImGui Demo
        if (imguiDemo_)
        {
            ImGui::ShowDemoWindow(&imguiDemo_);
        }
    }

public:
    float width_        = 1280.0f;
    float height_       = 1024.0f;
    float lastTimestep_ = 0.0f;

    glm::vec3 cameraPos_ {11.0f, 8.70f, 3.0f};
    glm::vec3 cameraLookAt_ {0.0f, 0.0f, 0.0f};
    float cameraFOV_ = 45.0f;

    ;
    float ambientLight_ = 0.1f;

    float modelScale_ = 0.15f;

    struct Light
    {
        struct Vertex
        {
            glm::vec3 position = {};
            glm::vec4 color    = {};
        };

        glm::vec3 position = {8.4f, 4.0f, 3.0f};
        glm::vec4 color    = {1.0f, 1.0f, 1.0f, 1.0f};
        float scale        = 0.5f;

        std::shared_ptr<moci::Shader> shader    = nullptr;
        std::shared_ptr<moci::VertexBuffer> vbo = nullptr;
        std::shared_ptr<moci::IndexBuffer> ibo  = nullptr;
        std::shared_ptr<moci::VertexArray> vao  = nullptr;
        std::vector<Light::Vertex> vertices     = {};
    };

    Light light {};

    std::shared_ptr<moci::Shader> shader_    = nullptr;
    std::shared_ptr<moci::VertexBuffer> vbo_ = nullptr;
    std::shared_ptr<moci::IndexBuffer> ibo_  = nullptr;
    std::shared_ptr<moci::VertexArray> vao_  = nullptr;

    std::size_t numVertices_ {};
    // Mesh mesh_ {"sandbox3D/assets/models/donut.obj"};
    Mesh mesh_ {"sandbox3D/assets/models/cerberus.fbx"};
    Mesh lightMesh_ {"sandbox3D/assets/models/sphere.obj"};
    Mesh floor_ {"sandbox3D/assets/models/plane.obj"};

    moci::Texture2D::Ptr textureSolid_ {};
    moci::Texture2D::Ptr textureColors_ {};

    std::vector<Vertex> vertices_ {};

    // imgui
    struct DrawStats
    {
        static constexpr std::size_t ResetRate = 1'000;
        std::uint32_t resetCounter {};
        std::uint32_t numVertices {};
        float minFPS = 9999.0f;
        float maxFPS = 0.0f;
    };

    DrawStats drawStats_ {};

    std::vector<float> fpsHistory_ = {};
    bool imguiWindow_              = true;
    bool imguiDemo_                = false;
};

class Sandbox : public moci::Application
{
public:
    Sandbox()
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox-3d", "moci-sandbox-3d.json");
        PushLayer(std::make_unique<DemoLayer>());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

auto moci::CreateApplication() -> moci::Application* { return new Sandbox(); }