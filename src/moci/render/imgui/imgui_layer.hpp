#pragma once

#include <moci/events/layer/layer.hpp>

#include <string>

namespace moci {

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    explicit ImGuiLayer(std::string const& name);
    ~ImGuiLayer() override;

    static void begin();
    static void end();

    void onAttach() override;
    void onDetach() override;
    void onImGuiRender() override;
};
}  // namespace moci
