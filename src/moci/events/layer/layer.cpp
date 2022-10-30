#include "layer.hpp"

#include <utility>

namespace moci
{

Layer::Layer(std::string debugName) : _debugName(std::move(debugName)) { }

}  // namespace moci