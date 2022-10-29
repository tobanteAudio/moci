#pragma once

#include "render_command.hpp"

#include "moci/core/memory.hpp"

namespace moci
{

class Renderer
{
public:
    static void init();
    static void onWindowResize(uint32_t width, uint32_t height);

    // static void BeginScene(OrthographicCamera& camera);
    // static void EndScene();

    // static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
    //                    const glm::mat4& transform = glm::mat4(1.0f));

    inline static auto getApi() -> RendererAPI::API { return RendererAPI::getApi(); }

private:
    // struct SceneData
    // {
    //     glm::mat4 ViewProjectionMatrix;
    // };

    // static Scope<SceneData> s_SceneData;
};
}  // namespace moci
