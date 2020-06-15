#pragma once

#include <gsl/gsl>

namespace moci
{
template<typename Type>
using Span = gsl::span<Type>;
}  // namespace moci
