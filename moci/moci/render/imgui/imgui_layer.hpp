#pragma once

#include <string>

#include "moci_graphics_api_opengles/moci_graphics_api_opengles.hpp"

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