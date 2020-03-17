#include "moci/render/opengles2/opengles2.hpp"

#include "examples/imgui_impl_glfw.cpp"

#if !defined(MOCI_MAC)
#include "examples/imgui_impl_opengl3.cpp"
#else
#include "examples/imgui_impl_opengl2.cpp"
#endif