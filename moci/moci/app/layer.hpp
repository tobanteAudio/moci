#pragma once

#include "events/event.hpp"
#include "timestep.hpp"

#include <memory>
#include <string>

namespace moci
{

class Layer
{
public:
    using Ptr = std::unique_ptr<Layer>;

public:
    Layer(std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}

    [[nodiscard]] inline auto GetName() const -> std::string const& { return m_DebugName; }

private:
    std::string m_DebugName;
};

}  // namespace moci