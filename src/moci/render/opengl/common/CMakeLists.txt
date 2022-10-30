project(moci_render_opengl_common)

set(moci_render_opengl_common_sources
    shader_parser.hpp
    shader_parser.cpp

)

add_library(moci_render_opengl_common_lib STATIC ${moci_render_opengl_common_sources})
add_library(Moci::RenderOpenGLCommon ALIAS moci_render_opengl_common_lib)
set_target_properties(moci_render_opengl_common_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_render_opengl_common_lib
    PUBLIC
        Moci::RenderApiBase
    PRIVATE
        Moci::CodeCoverage

)

set (moci_render_opengl_common_test_src
    main_test.cpp
    shader_parser_test.cpp
)
add_executable(moci_render_opengl_common_tests ${moci_render_opengl_common_test_src})
set_target_properties(moci_render_opengl_common_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_render_opengl_common_tests
    PRIVATE
        Moci::RenderOpenGLCommon
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_opengl_common_test_src})
catch_discover_tests(moci_render_opengl_common_tests)
