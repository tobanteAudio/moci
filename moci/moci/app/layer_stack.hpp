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

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    auto begin() -> std::vector<Layer*>::iterator { return m_Layers.begin(); }
    auto end() -> std::vector<Layer*>::iterator { return m_Layers.end(); }

private:
    std::vector<Layer*> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};

}  // namespace moci