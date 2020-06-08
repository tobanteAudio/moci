#pragma once

#if MOCI_API_OPENGL_MODERN

#include "moci_core/core/preprocessor.hpp"

#include "moci_core/core/logging.hpp"

#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#endif