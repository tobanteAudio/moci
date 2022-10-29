project(moci_render_opengl)

set(moci_render_opengl_sources
    moci_render_opengl.hpp
    moci_render_opengl.cpp

    gl4/buffer.hpp
    gl4/buffer.cpp
    gl4/frame_buffer.hpp
    gl4/frame_buffer.cpp
    gl4/graphics_context.hpp
    gl4/graphics_context.cpp
    gl4/gl4.hpp
    gl4/gl4.cpp
    gl4/renderer_api.hpp
    gl4/renderer_api.cpp
    gl4/shader.hpp
    gl4/shader.cpp
    gl4/texture.hpp
    gl4/texture.cpp
    gl4/vertex_array.hpp
    gl4/vertex_array.cpp

)

add_library(moci_render_opengl_lib STATIC ${moci_render_opengl_sources})
add_library(Moci::RenderOpenGL ALIAS moci_render_opengl_lib)
set_target_properties(moci_render_opengl_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_render_opengl_lib PUBLIC Moci::RenderOpenGLCommon)

if (NOT ${MOCI_LINK_OPENGL_ES})
    target_link_libraries(moci_render_opengl_lib PUBLIC GLEW::GLEW)
endif()