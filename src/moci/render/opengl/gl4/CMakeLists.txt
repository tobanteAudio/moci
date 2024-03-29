project(moci_render_opengl)

# Only include glew on desktop
if (NOT ${MOCI_LINK_OPENGL_ES})
    find_package(GLEW REQUIRED)
endif()


set(moci_render_opengl_sources
    buffer.hpp
    buffer.cpp
    frame_buffer.hpp
    frame_buffer.cpp
    graphics_context.hpp
    graphics_context.cpp
    gl4.hpp
    gl4.cpp
    renderer_api.hpp
    renderer_api.cpp
    shader.hpp
    shader.cpp
    texture.hpp
    texture.cpp
    vertex_array.hpp
    vertex_array.cpp

)

add_library(moci_render_opengl ${moci_render_opengl_sources})
add_library(Moci::RenderOpenGL ALIAS moci_render_opengl)
target_link_libraries(moci_render_opengl PUBLIC Moci::RenderOpenGLCommon)

if (NOT ${MOCI_LINK_OPENGL_ES})
    target_link_libraries(moci_render_opengl PUBLIC GLEW::GLEW)
endif()
