#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci/core/preprocessor.hpp"

#include "moci/core/logging.hpp"

// glfw use gles headers
#if defined(MOCI_API_OPENGL_ES)
// #define GLFW_INCLUDE_ES2
#endif

// macOS 10.14 deprecated warnings
#if defined(MOCI_MAC)
#error
#define GL_SILENCE_DEPRECATION
#endif

// glew is not needed for opengl es 2.0 or on macOS
#if defined(MOCI_API_OPENGL_ES)
#else
#if defined(MOCI_MAC)
#else
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#endif