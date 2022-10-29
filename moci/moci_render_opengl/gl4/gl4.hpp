#pragma once

#if MOCI_API_OPENGL_MODERN

#include "moci/core/logging.hpp"

#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#endif