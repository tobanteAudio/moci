#include "moci/render/mesh.hpp"

#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace moci
{

namespace
{
constexpr uint32_t s_MeshImportFlags = aiProcess_CalcTangentSpace |      // Create binormals/tangents just in case
                                       aiProcess_Triangulate |           // Make sure we're triangles
                                       aiProcess_SortByPType |           // Split meshes by primitive type
                                       aiProcess_GenNormals |            // Make sure we have legit normals
                                       aiProcess_GenUVCoords |           // Convert UVs if required
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

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
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
}  // namespace

Mesh::Mesh(std::string filePath) : filePath_(std::move(filePath))
{
    LogStream::Initialize();

    MOCI_CORE_INFO("Loading mesh: {0}", filePath_.c_str());

    importer_ = moci::MakeScope<Assimp::Importer>();

    const aiScene* scene = importer_->ReadFile(filePath_, s_MeshImportFlags);
    if ((scene == nullptr) || !scene->HasMeshes()) { MOCI_CORE_ERROR("Failed to load mesh file: {0}", filePath_);

}

    isAnimated_ = scene->mAnimations != nullptr;
    // m_MeshShader = isAnimated_ ? Renderer::GetShaderLibrary()->Get("HazelPBR_Anim")
    //                             : Renderer::GetShaderLibrary()->Get("HazelPBR_Static");
    // m_Material.reset(new moci::Material(m_MeshShader));
    inverseTransform_ = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));

    uint32_t vertexCount = 0;
    uint32_t indexCount  = 0;

    submeshes_.reserve(scene->mNumMeshes);
    for (size_t m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* mesh = scene->mMeshes[m];

        Submesh submesh{};
        submesh.BaseVertex    = vertexCount;
        submesh.BaseIndex     = indexCount;
        submesh.MaterialIndex = mesh->mMaterialIndex;
        submesh.IndexCount    = mesh->mNumFaces * 3;
        submeshes_.push_back(submesh);

        vertexCount += mesh->mNumVertices;
        indexCount += submesh.IndexCount;

        MOCI_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
        MOCI_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

        // Vertices
        if (isAnimated_)
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
                {
                    vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
                }

                staticVertices_.push_back(vertex);
            }
        }

        // Indices
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            MOCI_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
            indices_.push_back({mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]});
        }
    }

    // MOCI_CORE_TRACE("NODES:");
    // MOCI_CORE_TRACE("-----------------------------");
    // TraverseNodes(scene->mRootNode);
    // MOCI_CORE_TRACE("-----------------------------");

    // // Bones
    // if (isAnimated_)
    // {
    //     for (size_t m = 0; m < scene->mNumMeshes; m++)
    //     {
    //         aiMesh* mesh     = scene->mMeshes[m];
    //         Submesh& submesh = submeshes_[m];

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
    // if (isAnimated_)
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
    //     auto vb = VertexBuffer::Create(staticVertices_.data(), staticVertices_.size() * sizeof(Vertex));
    //     vb->SetLayout({
    //         {ShaderDataType::Float3, "a_Position"},
    //         {ShaderDataType::Float3, "a_Normal"},
    //         {ShaderDataType::Float3, "a_Tangent"},
    //         {ShaderDataType::Float3, "a_Binormal"},
    //         {ShaderDataType::Float2, "a_TexCoord"},
    //     });
    //     m_VertexArray->AddVertexBuffer(vb);
    // }

    // auto ib = IndexBuffer::Create(indices_.data(), indices_.size() * sizeof(Index));
    // m_VertexArray->SetIndexBuffer(ib);
    // m_Scene = scene;
}
}  // namespace moci