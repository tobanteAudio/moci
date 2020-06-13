#pragma once

#include <cstdint>

// From glfw3.h
namespace moci
{
enum class MouseCode : std::uint8_t
{
    Button1 = 0,
    Button2 = 1,
    Button3 = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7,

    ButtonLast   = 8,
    ButtonLeft   = 1,
    ButtonRight  = 2,
    ButtonMiddle = 3,
};
}