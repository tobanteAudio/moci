#pragma once

#include "layer.hpp"

#include "moci/core/vector.hpp"

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

    auto begin() -> Vector<Layer::Ptr>::iterator { return m_Layers.begin(); }
    auto end() -> Vector<Layer::Ptr>::iterator { return m_Layers.end(); }

private:
    Vector<Layer::Ptr> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};

}  // namespace moci