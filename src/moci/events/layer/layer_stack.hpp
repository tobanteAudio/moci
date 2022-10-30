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

    void pushLayer(Layer::Ptr&& layer);
    void pushOverlay(Layer::Ptr&& overlay);
    // void PopLayer(Layer* layer);
    // void PopOverlay(Layer* overlay);

    auto begin() -> Vector<Layer::Ptr>::iterator { return _layers.begin(); }
    auto end() -> Vector<Layer::Ptr>::iterator { return _layers.end(); }

private:
    Vector<Layer::Ptr> _layers;
    unsigned int _layerInsertIndex = 0;
};

}  // namespace moci