#include "es2.hpp"

void glClearError()
{
    while (glGetError() != GL_NO_ERROR) {
        ;
    }
}

auto glLogCall(char const* function, char const* file, int line) -> bool
{
    while (GLenum error = glGetError()) {
        MOCI_CORE_ERROR("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line);
        return false;
    }
    return true;
}
