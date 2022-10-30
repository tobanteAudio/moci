#include "layer_stack.hpp"

#include <algorithm>
#include <utility>

namespace moci
{

LayerStack::LayerStack() = default;

LayerStack::~LayerStack()
{
    for (Layer::Ptr& layer : _layers) { layer->onDetach(); }
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

// void LayerStack::PopLayer(Layer::Ptr&& layer)
// {
//     auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
//     if (it != m_Layers.begin() + m_LayerInsertIndex)
//     {
//         layer->OnDetach();
//         m_Layers.erase(it);
//         m_LayerInsertIndex--;
//     }
// }

// void LayerStack::PopOverlay(Layer::Ptr&& overlay)
// {
//     auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
//     if (it != m_Layers.end())
//     {
//         overlay->OnDetach();
//         m_Layers.erase(it);
//     }
// }

}  // namespace moci
