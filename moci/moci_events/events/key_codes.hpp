
#pragma once

#include <cstdint>
#include <iostream>

namespace moci
{
// From glfw3.h
enum class KeyCode : uint16_t
{
    Space      = 32,
    Apostrophe = 39, /* ' */
    Comma      = 44, /* , */
    Minus      = 45, /* - */
    Period     = 46, /* . */
    Slash      = 47, /* / */

    D0 = 48, /* 0 */
    D1 = 49, /* 1 */
    D2 = 50, /* 2 */
    D3 = 51, /* 3 */
    D4 = 52, /* 4 */
    D5 = 53, /* 5 */
    D6 = 54, /* 6 */
    D7 = 55, /* 7 */
    D8 = 56, /* 8 */
    D9 = 57, /* 9 */

    Semicolon = 59, /* ; */
    Equal     = 61, /* = */

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    LeftBracket  = 91, /* [ */
    Backslash    = 92, /* \ */
    RightBracket = 93, /* ] */
    GraveAccent  = 96, /* ` */

    World1 = 161, /* non-US #1 */
    World2 = 162, /* non-US #2 */

    /* Function keys */
    Escape      = 256,
    Enter       = 257,
    Tab         = 258,
    Backspace   = 259,
    Insert      = 260,
    Delete      = 261,
    Right       = 262,
    Left        = 263,
    Down        = 264,
    Up          = 265,
    PageUp      = 266,
    PageDown    = 267,
    Home        = 268,
    End         = 269,
    CapsLock    = 280,
    ScrollLock  = 281,
    NumLock     = 282,
    PrintScreen = 283,
    Pause       = 284,
    F1          = 290,
    F2          = 291,
    F3          = 292,
    F4          = 293,
    F5          = 294,
    F6          = 295,
    F7          = 296,
    F8          = 297,
    F9          = 298,
    F10         = 299,
    F11         = 300,
    F12         = 301,
    F13         = 302,
    F14         = 303,
    F15         = 304,
    F16         = 305,
    F17         = 306,
    F18         = 307,
    F19         = 308,
    F20         = 309,
    F21         = 310,
    F22         = 311,
    F23         = 312,
    F24         = 313,
    F25         = 314,

    /* Keypad */
    KP0        = 320,
    KP1        = 321,
    KP2        = 322,
    KP3        = 323,
    KP4        = 324,
    KP5        = 325,
    KP6        = 326,
    KP7        = 327,
    KP8        = 328,
    KP9        = 329,
    KPDecimal  = 330,
    KPDivide   = 331,
    KPMultiply = 332,
    KPSubtract = 333,
    KPAdd      = 334,
    KPEnter    = 335,
    KPEqual    = 336,

    LeftShift    = 340,
    LeftControl  = 341,
    LeftAlt      = 342,
    LeftSuper    = 343,
    RightShift   = 344,
    RightControl = 345,
    RightAlt     = 346,
    RightSuper   = 347,
    Menu         = 348
};

using Key = KeyCode;

inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
{
    os << static_cast<int32_t>(keyCode);
    return os;
}
}  // namespace moci

// clang-format off
// From glfw3.h
#define MOCI_KEY_SPACE           ::moci::Key::Space
#define MOCI_KEY_APOSTROPHE      ::moci::Key::Apostrophe    /* ' */
#define MOCI_KEY_COMMA           ::moci::Key::Comma         /* , */
#define MOCI_KEY_MINUS           ::moci::Key::Minus         /* - */
#define MOCI_KEY_PERIOD          ::moci::Key::Period        /* . */
#define MOCI_KEY_SLASH           ::moci::Key::Slash         /* / */
#define MOCI_KEY_0               ::moci::Key::D0
#define MOCI_KEY_1               ::moci::Key::D1
#define MOCI_KEY_2               ::moci::Key::D2
#define MOCI_KEY_3               ::moci::Key::D3
#define MOCI_KEY_4               ::moci::Key::D4
#define MOCI_KEY_5               ::moci::Key::D5
#define MOCI_KEY_6               ::moci::Key::D6
#define MOCI_KEY_7               ::moci::Key::D7
#define MOCI_KEY_8               ::moci::Key::D8
#define MOCI_KEY_9               ::moci::Key::D9
#define MOCI_KEY_SEMICOLON       ::moci::Key::Semicolon     /* ; */
#define MOCI_KEY_EQUAL           ::moci::Key::Equal         /* = */
#define MOCI_KEY_A               ::moci::Key::A
#define MOCI_KEY_B               ::moci::Key::B
#define MOCI_KEY_C               ::moci::Key::C
#define MOCI_KEY_D               ::moci::Key::D
#define MOCI_KEY_E               ::moci::Key::E
#define MOCI_KEY_F               ::moci::Key::F
#define MOCI_KEY_G               ::moci::Key::G
#define MOCI_KEY_H               ::moci::Key::H
#define MOCI_KEY_I               ::moci::Key::I
#define MOCI_KEY_J               ::moci::Key::J
#define MOCI_KEY_K               ::moci::Key::K
#define MOCI_KEY_L               ::moci::Key::L
#define MOCI_KEY_M               ::moci::Key::M
#define MOCI_KEY_N               ::moci::Key::N
#define MOCI_KEY_O               ::moci::Key::O
#define MOCI_KEY_P               ::moci::Key::P
#define MOCI_KEY_Q               ::moci::Key::Q
#define MOCI_KEY_R               ::moci::Key::R
#define MOCI_KEY_S               ::moci::Key::S
#define MOCI_KEY_T               ::moci::Key::T
#define MOCI_KEY_U               ::moci::Key::U
#define MOCI_KEY_V               ::moci::Key::V
#define MOCI_KEY_W               ::moci::Key::W
#define MOCI_KEY_X               ::moci::Key::X
#define MOCI_KEY_Y               ::moci::Key::Y
#define MOCI_KEY_Z               ::moci::Key::Z
#define MOCI_KEY_LEFT_BRACKET    ::moci::Key::LeftBracket   /* [ */
#define MOCI_KEY_BACKSLASH       ::moci::Key::Backslash     /* \ */
#define MOCI_KEY_RIGHT_BRACKET   ::moci::Key::RightBracket  /* ] */
#define MOCI_KEY_GRAVE_ACCENT    ::moci::Key::GraveAccent   /* ` */
#define MOCI_KEY_WORLD_1         ::moci::Key::World1        /* non-US #1 */
#define MOCI_KEY_WORLD_2         ::moci::Key::World2        /* non-US #2 */

/* Function keys */
#define MOCI_KEY_ESCAPE          ::moci::Key::Escape
#define MOCI_KEY_ENTER           ::moci::Key::Enter
#define MOCI_KEY_TAB             ::moci::Key::Tab
#define MOCI_KEY_BACKSPACE       ::moci::Key::Backspace
#define MOCI_KEY_INSERT          ::moci::Key::Insert
#define MOCI_KEY_DELETE          ::moci::Key::Delete
#define MOCI_KEY_RIGHT           ::moci::Key::Right
#define MOCI_KEY_LEFT            ::moci::Key::Left
#define MOCI_KEY_DOWN            ::moci::Key::Down
#define MOCI_KEY_UP              ::moci::Key::Up
#define MOCI_KEY_PAGE_UP         ::moci::Key::PageUp
#define MOCI_KEY_PAGE_DOWN       ::moci::Key::PageDown
#define MOCI_KEY_HOME            ::moci::Key::Home
#define MOCI_KEY_END             ::moci::Key::End
#define MOCI_KEY_CAPS_LOCK       ::moci::Key::CapsLock
#define MOCI_KEY_SCROLL_LOCK     ::moci::Key::ScrollLock
#define MOCI_KEY_NUM_LOCK        ::moci::Key::NumLock
#define MOCI_KEY_PRINT_SCREEN    ::moci::Key::PrintScreen
#define MOCI_KEY_PAUSE           ::moci::Key::Pause
#define MOCI_KEY_F1              ::moci::Key::F1
#define MOCI_KEY_F2              ::moci::Key::F2
#define MOCI_KEY_F3              ::moci::Key::F3
#define MOCI_KEY_F4              ::moci::Key::F4
#define MOCI_KEY_F5              ::moci::Key::F5
#define MOCI_KEY_F6              ::moci::Key::F6
#define MOCI_KEY_F7              ::moci::Key::F7
#define MOCI_KEY_F8              ::moci::Key::F8
#define MOCI_KEY_F9              ::moci::Key::F9
#define MOCI_KEY_F10             ::moci::Key::F10
#define MOCI_KEY_F11             ::moci::Key::F11
#define MOCI_KEY_F12             ::moci::Key::F12
#define MOCI_KEY_F13             ::moci::Key::F13
#define MOCI_KEY_F14             ::moci::Key::F14
#define MOCI_KEY_F15             ::moci::Key::F15
#define MOCI_KEY_F16             ::moci::Key::F16
#define MOCI_KEY_F17             ::moci::Key::F17
#define MOCI_KEY_F18             ::moci::Key::F18
#define MOCI_KEY_F19             ::moci::Key::F19
#define MOCI_KEY_F20             ::moci::Key::F20
#define MOCI_KEY_F21             ::moci::Key::F21
#define MOCI_KEY_F22             ::moci::Key::F22
#define MOCI_KEY_F23             ::moci::Key::F23
#define MOCI_KEY_F24             ::moci::Key::F24
#define MOCI_KEY_F25             ::moci::Key::F25

/* Keypad */
#define MOCI_KEY_KP_0            ::moci::Key::KP0
#define MOCI_KEY_KP_1            ::moci::Key::KP1
#define MOCI_KEY_KP_2            ::moci::Key::KP2
#define MOCI_KEY_KP_3            ::moci::Key::KP3
#define MOCI_KEY_KP_4            ::moci::Key::KP4
#define MOCI_KEY_KP_5            ::moci::Key::KP5
#define MOCI_KEY_KP_6            ::moci::Key::KP6
#define MOCI_KEY_KP_7            ::moci::Key::KP7
#define MOCI_KEY_KP_8            ::moci::Key::KP8
#define MOCI_KEY_KP_9            ::moci::Key::KP9
#define MOCI_KEY_KP_DECIMAL      ::moci::Key::KPDecimal
#define MOCI_KEY_KP_DIVIDE       ::moci::Key::KPDivide
#define MOCI_KEY_KP_MULTIPLY     ::moci::Key::KPMultiply
#define MOCI_KEY_KP_SUBTRACT     ::moci::Key::KPSubtract
#define MOCI_KEY_KP_ADD          ::moci::Key::KPAdd
#define MOCI_KEY_KP_ENTER        ::moci::Key::KPEnter
#define MOCI_KEY_KP_EQUAL        ::moci::Key::KPEqual

#define MOCI_KEY_LEFT_SHIFT      ::moci::Key::LeftShift
#define MOCI_KEY_LEFT_CONTROL    ::moci::Key::LeftControl
#define MOCI_KEY_LEFT_ALT        ::moci::Key::LeftAlt
#define MOCI_KEY_LEFT_SUPER      ::moci::Key::LeftSuper
#define MOCI_KEY_RIGHT_SHIFT     ::moci::Key::RightShift
#define MOCI_KEY_RIGHT_CONTROL   ::moci::Key::RightControl
#define MOCI_KEY_RIGHT_ALT       ::moci::Key::RightAlt
#define MOCI_KEY_RIGHT_SUPER     ::moci::Key::RightSuper
#define MOCI_KEY_MENU            ::moci::Key::Menu
// clang-format on