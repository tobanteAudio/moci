#pragma once

#include <moci/events/timestep.hpp>
#include <moci/events/types/event.hpp>

#include <memory>
#include <string>

namespace moci {

class Layer
{
public:
    using Ptr = std::unique_ptr<Layer>;

    explicit Layer(std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void onAttach() {}

    virtual void onDetach() {}

    virtual void onUpdate(Timestep ts) { ignoreUnused(ts); }

    virtual void onImGuiRender() {}

    virtual void onEvent(Event& event) { ignoreUnused(event); }

    [[nodiscard]] inline auto getName() const -> std::string const& { return _debugName; }

private:
    std::string _debugName;
};

}  // namespace moci
