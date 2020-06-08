#pragma once

#include "moci_core/core/vector.hpp"

#include <algorithm>
#include <iterator>
#include <tuple>
namespace moci
{
/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.

bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1

*/
template<typename T>
std::pair<bool, int> FindInVector(const Vector<T>& vec, const T& element)
{
    std::pair<bool, int> result;

    // Find given element in vector
    auto it = std::find(std::begin(vec), std::end(vec), element);

    if (it != std::end(vec))
    {
        result.second = std::distance(std::begin(vec), it);
        result.first  = true;
    }
    else
    {
        result.first  = false;
        result.second = -1;
    }

    return result;
}
}  // namespace moci