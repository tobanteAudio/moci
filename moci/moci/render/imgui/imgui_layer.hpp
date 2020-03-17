#pragma once

#include <string>

#include "moci/render/opengles2/opengles2.hpp"

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

private:
    bool m_show_imgui_demo = false;
};
}  // namespace moci