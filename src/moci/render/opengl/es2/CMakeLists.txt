project(moci_render_opengles)

# Only include glew on desktop
if (NOT ${MOCI_LINK_OPENGL_ES})
    find_package(GLEW REQUIRED)
endif()


set(moci_render_opengles_sources
    buffer.hpp
    buffer.cpp
    frame_buffer.hpp
    frame_buffer.cpp
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

add_library(moci_render_opengles STATIC ${moci_render_opengles_sources})
add_library(Moci::RenderOpenGLES ALIAS moci_render_opengles)
set_target_properties(moci_render_opengles PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_render_opengles PUBLIC Moci::RenderOpenGLCommon)
target_include_directories(moci_render_opengles PUBLIC /opt/vc/include)

if (${MOCI_LINK_OPENGL_ES})
    # old
    # target_link_libraries(moci_render_opengles PUBLIC -lGLESv2)
    # raspberry pi 3
    # target_link_libraries(moci_render_opengles PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    target_link_libraries(moci_render_opengles PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    target_compile_definitions(moci_render_opengles PUBLIC MOCI_API_OPENGL_ES=1 MOCI_API_OPENGL_LEGACY=1)
else()
    target_link_libraries(moci_render_opengles PUBLIC GLEW::GLEW)
endif()