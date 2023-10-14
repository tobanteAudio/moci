#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

auto main() -> int
{
    if (auto success = glfwInit(); success == GLFW_FALSE) {
        std::fprintf(stderr, "Failed to initialize glfw\n");
        return EXIT_FAILURE;
    }

    auto *window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
    glfwHideWindow(window);
    glfwMakeContextCurrent(window);

    if (auto err = glewInit() != GLEW_OK; err) {
        auto const* msg = glewGetErrorString(static_cast<GLenum>(err));
        std::fprintf(
            stderr,
            "Failed to initialize glew: %s\n",
            reinterpret_cast<char const*>(msg)
        );
        return EXIT_FAILURE;
    }

    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    std::printf("num: %d\n", n);

    for (GLint i = 0; i < n; i++) {
        char const* extension = (char const*)glGetStringi(GL_EXTENSIONS, i);
        printf("Ext %d: %s\n", i, extension);
    }

    return EXIT_SUCCESS;
}
