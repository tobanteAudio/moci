#include <moci/core/preprocessor.hpp>

// glew is not needed for opengl es 2.0
#if defined(MOCI_API_OPENGL_ES)
    #define GLFW_INCLUDE_ES2
    #define IMGUI_IMPL_OPENGL_ES2
    #define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#else
    #define IMGUI_IMPL_OPENGL_LOADER_GLEW
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "imgui.h"

#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"
