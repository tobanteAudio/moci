#include "moci_core/core/preprocessor.hpp"

// glew is not needed for opengl es 2.0
#if defined(MOCI_API_OPENGL_ES)
#define GLFW_INCLUDE_ES2
#define IMGUI_IMPL_OPENGL_ES2
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#else
#if defined(MOCI_MAC)
#define GL_SILENCE_DEPRECATION
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#else
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#include "imgui.h"

#include "examples/imgui_impl_glfw.h"

#if defined(MOCI_MAC)
#include "examples/imgui_impl_opengl2.h"
#else
#include "examples/imgui_impl_opengl3.h"
#endif