#pragma once

#include "layer.hpp"

#include <vector>

namespace moci
{

class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer::Ptr&& layer);
    void PushOverlay(Layer::Ptr&& overlay);
    // void PopLayer(Layer* layer);
    // void PopOverlay(Layer* overlay);

    auto begin() -> std::vector<Layer::Ptr>::iterator { return m_Layers.begin(); }
    auto end() -> std::vector<Layer::Ptr>::iterator { return m_Layers.end(); }

private:
    std::vector<Layer::Ptr> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};

}  // namespace moci