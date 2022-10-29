#include "layer.hpp"

#include <utility>

namespace moci
{

Layer::Layer(std::string debugName) : _m_DebugName(std::move(debugName)) { }

}  // namespace moci