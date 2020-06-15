#pragma once

#if MOCI_API_OPENGL_MODERN

#include "moci_core/moci_core.hpp"

#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#endif