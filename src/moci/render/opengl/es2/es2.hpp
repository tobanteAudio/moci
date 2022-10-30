#pragma once

#include "moci/core/preprocessor.hpp"

#include "moci/core/logging.hpp"

// glfw use gles headers
#if defined(MOCI_API_OPENGL_ES)
#define GLFW_INCLUDE_ES2
#endif

// macOS 10.14 deprecated warnings
#if defined(MOCI_MAC)
#define GL_SILENCE_DEPRECATION
#endif

// glew is not needed for opengl es 2.0 or on macOS
#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#define GLCall(x)                                                                                                      \
    glClearError();                                                                                                    \
    x;                                                                                                                 \
    glLogCall(#x, __FILE__, __LINE__)

void glClearError();
auto glLogCall(const char* function, const char* file, int line) -> bool;