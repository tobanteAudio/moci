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

    void pushLayer(Layer::Ptr&& layer);
    void pushOverlay(Layer::Ptr&& overlay);
    // void PopLayer(Layer* layer);
    // void PopOverlay(Layer* overlay);

    auto begin() -> std::vector<Layer::Ptr>::iterator { return _layers.begin(); }
    auto end() -> std::vector<Layer::Ptr>::iterator { return _layers.end(); }

private:
    std::vector<Layer::Ptr> _layers;
    unsigned int _layerInsertIndex = 0;
};

}  // namespace moci