#pragma once

#include <cstdint>
#include <iostream>

namespace moci {
// From glfw3.h
enum class MouseCode : uint16_t
{
    Button0 = 0,
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    Button4 = 4,
    Button5 = 5,
    Button6 = 6,
    Button7 = 7,

    ButtonLast   = Button7,
    ButtonLeft   = Button0,
    ButtonRight  = Button1,
    ButtonMiddle = Button2
};

using Mouse = MouseCode;

inline auto operator<<(std::ostream& out, MouseCode mouseCode) -> std::ostream&
{
    out << static_cast<int32_t>(mouseCode);
    return out;
}
}  // namespace moci
