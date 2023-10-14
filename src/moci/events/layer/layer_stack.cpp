#include "layer_stack.hpp"

#include <algorithm>
#include <utility>

namespace moci {

LayerStack::LayerStack() = default;

LayerStack::~LayerStack()
{
    for (Layer::Ptr& layer : _layers) {
        layer->onDetach();
    }
}

void LayerStack::pushLayer(Layer::Ptr&& layer)
{
    auto& item = *_layers.insert(_layers.begin() + _layerInsertIndex, std::move(layer));
    _layerInsertIndex++;
    item->onAttach();
}

void LayerStack::pushOverlay(Layer::Ptr&& overlay)
{
    overlay->onAttach();
    _layers.push_back(std::move(overlay));
}

}  // namespace moci
