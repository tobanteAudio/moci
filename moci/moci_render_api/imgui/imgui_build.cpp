#include "moci_render_opengles/moci_render_opengles.hpp"

#include "moci_render_api/imgui/imgui.hpp"

#include "backends/imgui_impl_glfw.cpp"

#if !defined(MOCI_MAC)
#include "backends/imgui_impl_opengl3.cpp"
#else
#include "backends/imgui_impl_opengl2.cpp"
#endif