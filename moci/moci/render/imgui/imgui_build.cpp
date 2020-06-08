#include "moci_graphics_api_opengles/moci_graphics_api_opengles.hpp"

#include "moci/render/imgui/imgui.hpp"

#include "examples/imgui_impl_glfw.cpp"

#if !defined(MOCI_MAC)
#include "examples/imgui_impl_opengl3.cpp"
#else
#include "examples/imgui_impl_opengl2.cpp"
#endif