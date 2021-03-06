#include "layer_stack.hpp"

#include <algorithm>
#include <utility>

namespace moci
{

LayerStack::LayerStack() = default;

LayerStack::~LayerStack()
{
    for (Layer::Ptr& layer : m_Layers) { layer->OnDetach(); }
}

void LayerStack::PushLayer(Layer::Ptr&& layer)
{
    auto& item = *m_Layers.insert(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
    m_LayerInsertIndex++;
    item->OnAttach();
}

void LayerStack::PushOverlay(Layer::Ptr&& overlay)
{
    overlay->OnAttach();
    m_Layers.push_back(std::move(overlay));
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
