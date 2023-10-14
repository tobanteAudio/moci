#include "mesh.hpp"

#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace moci
{

namespace
{
constexpr uint32_t SMeshImportFlags = aiProcess_CalcTangentSpace |      // Create binormals/tangents just in case
                                      aiProcess_Triangulate |           // Make sure we're triangles
                                      aiProcess_SortByPType |           // Split meshes by primitive type
                                      aiProcess_GenNormals |            // Make sure we have legit normals
                                      aiProcess_GenUVCoords |           // Convert UVs if required
                                      aiProcess_OptimizeMeshes |        // Batch draws where possible
                                      aiProcess_ValidateDataStructure;  // Validation

struct LogStream : public Assimp::LogStream
{
    static void initialize()
    {
        if (Assimp::DefaultLogger::isNullLogger())
        {
            Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
            Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
        }
    }

    void write(const char* message) override { MOCI_CORE_ERROR("Assimp error: {0}", message); }
};

auto aiMatrix4x4ToGlm(const aiMatrix4x4& from) -> glm::mat4
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

Mesh::Mesh(std::string filePath) : _filePath(std::move(filePath))
{
    LogStream::initialize();

    MOCI_CORE_INFO("Loading mesh: {0}", _filePath.c_str());

    _importer = std::make_unique<Assimp::Importer>();

    const aiScene* scene = _importer->ReadFile(_filePath, SMeshImportFlags);
    if ((scene == nullptr) || !scene->HasMeshes()) { MOCI_CORE_ERROR("Failed to load mesh file: {0}", _filePath); }

    _isAnimated       = scene->mAnimations != nullptr;
    _inverseTransform = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));

    uint32_t vertexCount = 0;
    uint32_t indexCount  = 0;

    _submeshes.reserve(scene->mNumMeshes);
    for (size_t m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* mesh = scene->mMeshes[m];

        Submesh submesh {};
        submesh.BaseVertex    = vertexCount;
        submesh.BaseIndex     = indexCount;
        submesh.MaterialIndex = mesh->mMaterialIndex;
        submesh.IndexCount    = mesh->mNumFaces * 3;
        _submeshes.push_back(submesh);

        vertexCount += mesh->mNumVertices;
        indexCount += submesh.IndexCount;

        MOCI_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
        MOCI_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

        // Vertices
        if (not _isAnimated)
        {
            for (size_t i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                vertex.normal   = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
                vertex.color    = static_cast<glm::vec4>(moci::Colors::blue);

                if (mesh->HasTextureCoords(0))
                {
                    vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
                }

                _staticVertices.push_back(vertex);
            }
        }

        // Indices
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            MOCI_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
            _indices.push_back({mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]});
        }
    }
}
}  // namespace moci