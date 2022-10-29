#pragma once

#include <string>

#include "moci_render_opengles/moci_render_opengles.hpp"

#include "moci_render_api/moci_render_api.hpp"

namespace moci
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    explicit ImGuiLayer(std::string const& name);
    ~ImGuiLayer() override;

    static void Begin();
    static void End();

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;
};
}  // namespace moci