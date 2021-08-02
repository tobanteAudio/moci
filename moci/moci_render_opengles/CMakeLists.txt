project(moci_render_opengles)

set(moci_render_opengles_sources
    moci_render_opengles.hpp
    moci_render_opengles.cpp

    es2/buffer.hpp
    es2/buffer.cpp
    es2/frame_buffer.hpp
    es2/frame_buffer.cpp
    es2/graphics_context.hpp
    es2/graphics_context.cpp
    es2/es2.hpp
    es2/es2.cpp
    es2/renderer_api.hpp
    es2/renderer_api.cpp
    es2/shader.hpp
    es2/shader.cpp
    es2/texture.hpp
    es2/texture.cpp
    es2/vertex_array.hpp
    es2/vertex_array.cpp

)

add_library(moci_render_opengles_lib STATIC ${moci_render_opengles_sources})
add_library(Moci::RenderOpenGLES ALIAS moci_render_opengles_lib)
target_link_libraries(moci_render_opengles_lib PUBLIC Moci::RenderOpenGLCommon)
target_include_directories(moci_render_opengles_lib PUBLIC /opt/vc/include)

if (${MOCI_LINK_OPENGL_ES})
    # old
    # target_link_libraries(moci_render_opengles_lib PUBLIC -lGLESv2)
    # raspberry pi 3
    # target_link_libraries(moci_render_opengles_lib PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    target_link_libraries(moci_render_opengles_lib PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    target_compile_definitions(moci_render_opengles_lib PUBLIC MOCI_API_OPENGL_ES=1 MOCI_API_OPENGL_LEGACY=1)
else()
    target_link_libraries(moci_render_opengles_lib PUBLIC GLEW::GLEW)
endif()