#pragma once

#include "moci/events/layer/layer.hpp"

#include <string>

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