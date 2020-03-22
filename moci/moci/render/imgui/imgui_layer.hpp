#pragma once

#include <string>

#include "moci/render/opengl/es2/es2.hpp"

#include "moci/app/layer.hpp"

namespace moci
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ImGuiLayer(std::string const& name);
    ~ImGuiLayer() override;

    static void Begin();
    static void End();

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;
};
}  // namespace moci