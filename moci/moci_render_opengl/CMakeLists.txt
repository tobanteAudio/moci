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

if (${MOCI_API_OPENGL_MODERN})
    target_compile_definitions(moci_render_opengl_lib PUBLIC MOCI_API_OPENGL_MODERN=1)
endif()

if (${MOCI_API_OPENGL_LEGACY})
    target_compile_definitions(moci_render_opengl_lib PUBLIC MOCI_API_OPENGL_LEGACY=1)
endif()

if (${MOCI_API_OPENGL_ES})
    # old
    # target_link_libraries(moci_render_opengl_lib PUBLIC -lGLESv2)
    # raspberry pi 3
    target_link_libraries(moci_render_opengl_lib PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    # target_link_libraries(moci_render_opengl_lib PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    
    target_compile_definitions(moci_render_opengl_lib PUBLIC MOCI_API_OPENGL_ES=1)
else()
    target_link_libraries(moci_render_opengl_lib PUBLIC libglew_static)
endif()

# set (moci_render_opengl_test_src
#     main_test.cpp

#     render/buffer_test.cpp
# )
# add_executable(moci_render_opengl_tests ${moci_render_opengl_test_src})
# target_link_libraries(moci_render_opengl_tests PRIVATE Moci::RenderOpenGL Catch2::Catch2)
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_opengl_test_src})
# catch_discover_tests(moci_render_opengl_tests)


