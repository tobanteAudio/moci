#include "moci_render_opengles/es2/es2.hpp"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR) { ; }
}

auto GLLogCall(const char* function, const char* file, int line) -> bool
{
    while (GLenum error = glGetError())
    {
        MOCI_CORE_ERROR("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line);
        return false;
    }
    return true;
}