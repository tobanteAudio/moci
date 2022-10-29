project(moci_render_base)

set(moci_render_base_sources
    moci_render_base.hpp
    moci_render_base.cpp

    render/api.hpp
    render/buffer.hpp
    render/buffer.cpp
    render/frame_buffer.hpp
    render/graphics_context.hpp
    render/graphics_context.cpp
    render/shader.hpp
    render/texture.hpp
    render/vertex_array.hpp

)

add_library(moci_render_base_lib STATIC ${moci_render_base_sources})
add_library(Moci::RenderApiBase ALIAS moci_render_base_lib)
set_target_properties(moci_render_base_lib PROPERTIES UNITY_BUILD OFF)

target_link_libraries(moci_render_base_lib
    PUBLIC
        Moci::GraphicsBase
        glfw
    PRIVATE
        Moci::CodeCoverage

)

if (${MOCI_API_OPENGL_MODERN})
    target_compile_definitions(moci_render_base_lib PUBLIC MOCI_API_OPENGL_MODERN=1)
endif()

if (${MOCI_API_OPENGL_LEGACY})
    target_compile_definitions(moci_render_base_lib PUBLIC MOCI_API_OPENGL_LEGACY=1)
endif()


set (moci_render_base_test_src main_test.cpp render/buffer_test.cpp)
add_executable(moci_render_base_tests ${moci_render_base_test_src})
set_target_properties(moci_render_base_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_render_base_tests
    PRIVATE
        Moci::RenderApiBase
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_base_test_src})
catch_discover_tests(moci_render_base_tests)
