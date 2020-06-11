project(moci_render_opengles)

set(moci_render_opengles_sources
    moci_render_opengles.hpp
    moci_render_opengles.cpp

    buffer.hpp
    buffer.cpp
    graphics_context.hpp
    graphics_context.cpp
    es2.hpp
    es2.cpp
    renderer_api.hpp
    renderer_api.cpp
    shader.hpp
    shader.cpp
    texture.hpp
    texture.cpp
    vertex_array.hpp
    vertex_array.cpp

)

add_library(moci_render_opengles_lib STATIC ${moci_render_opengles_sources})
add_library(Moci::RenderOpenGLES ALIAS moci_render_opengles_lib)
target_link_libraries(moci_render_opengles_lib PUBLIC Moci::RenderApiBase)
target_include_directories(moci_render_opengles_lib PUBLIC /opt/vc/include)

if (${MOCI_LINK_OPENGL_ES})
    # old
    # target_link_libraries(moci_render_opengles_lib PUBLIC -lGLESv2)
    # raspberry pi 3
    target_link_libraries(moci_render_opengles_lib PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    # target_link_libraries(moci_render_opengles_lib PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    
    target_compile_definitions(moci_render_opengles_lib PUBLIC MOCI_LINK_OPENGL_ES=1)
else()
    target_link_libraries(moci_render_opengles_lib PUBLIC libglew_static)
endif()