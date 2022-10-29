#pragma once

#include "moci/core/memory.hpp"
#include "moci/events/timestep.hpp"
#include "moci/events/types/event.hpp"

#include <string>

namespace moci
{

class Layer
{
public:
    using Ptr = Scope<Layer>;

    explicit Layer(std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void onAttach() { }
    virtual void onDetach() { }
    virtual void onUpdate(Timestep ts) { ignoreUnused(ts); }
    virtual void onImGuiRender() { }
    virtual void onEvent(Event& event) { ignoreUnused(event); }

    [[nodiscard]] inline auto getName() const -> std::string const& { return m_DebugName; }

private:
    std::string m_DebugName;
};

}  // namespace moci