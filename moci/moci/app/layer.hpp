#pragma once

#include "moci/core/memory.hpp"
#include "moci/app/events/event.hpp"
#include "moci/app/timestep.hpp"

#include <memory>
#include <string>

namespace moci
{

class Layer
{
public:
    using Ptr = Scope<Layer>;

public:
    Layer(std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) { IgnoreUnused(ts); }
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) { IgnoreUnused(event); }

    [[nodiscard]] inline auto GetName() const -> std::string const& { return m_DebugName; }

private:
    std::string m_DebugName;
};

}  // namespace moci