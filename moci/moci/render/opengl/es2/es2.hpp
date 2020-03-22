#pragma once

#include "moci/core/preprocessor.hpp"

#include "moci/core/logging.hpp"

// glfw use gles headers
#if defined(MOCI_GRAPHICS_API_GLES)
#define GLFW_INCLUDE_ES2
#endif

// macOS 10.14 deprecated warnings
#if defined(MOCI_MAC)
#define GL_SILENCE_DEPRECATION
#endif

// glew is not needed for opengl es 2.0 or on macOS
#if defined(MOCI_GRAPHICS_API_GLES)
#else
#if defined(MOCI_MAC)
#else
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#define GLCall(x)                                                                                                      \
    GLClearError();                                                                                                    \
    x;                                                                                                                 \
    GLLogCall(#x, __FILE__, __LINE__)

void GLClearError();
auto GLLogCall(const char* function, const char* file, int line) -> bool;