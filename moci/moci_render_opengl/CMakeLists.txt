project(moci_render_opengl)

set(moci_render_opengl_sources
    moci_render_opengl.hpp
    moci_render_opengl.cpp

    buffer.hpp
    buffer.cpp
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

add_library(moci_render_opengl_lib STATIC ${moci_render_opengl_sources})
add_library(Moci::RenderOpenGL ALIAS moci_render_opengl_lib)
target_link_libraries(moci_render_opengl_lib PUBLIC Moci::RenderApiBase)



