#pragma once

#include <cstdint>
#include <iostream>

namespace moci
{
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

#define MOCI_MOUSE_BUTTON_0 ::moci::Mouse::Button0
#define MOCI_MOUSE_BUTTON_1 ::moci::Mouse::Button1
#define MOCI_MOUSE_BUTTON_2 ::moci::Mouse::Button2
#define MOCI_MOUSE_BUTTON_3 ::moci::Mouse::Button3
#define MOCI_MOUSE_BUTTON_4 ::moci::Mouse::Button4
#define MOCI_MOUSE_BUTTON_5 ::moci::Mouse::Button5
#define MOCI_MOUSE_BUTTON_6 ::moci::Mouse::Button6
#define MOCI_MOUSE_BUTTON_7 ::moci::Mouse::Button7
#define MOCI_MOUSE_BUTTON_LAST ::moci::Mouse::ButtonLast
#define MOCI_MOUSE_BUTTON_LEFT ::moci::Mouse::ButtonLeft
#define MOCI_MOUSE_BUTTON_RIGHT ::moci::Mouse::ButtonRight
#define MOCI_MOUSE_BUTTON_MIDDLE ::moci::Mouse::ButtonMiddle