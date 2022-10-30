project(moci_render)

find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)

set(moci_render_sources
    api.hpp
    buffer.hpp
    buffer.cpp
    frame_buffer.hpp
    graphics_context.hpp
    graphics_context.cpp
    shader.hpp
    texture.hpp
    vertex_array.hpp

)

add_library(moci_render STATIC ${moci_render_sources})
add_library(Moci::RenderApiBase ALIAS moci_render)
set_target_properties(moci_render PROPERTIES UNITY_BUILD OFF)

target_link_libraries(moci_render
    PUBLIC
        Moci::Graphics
        glfw
    PRIVATE
        Moci::CodeCoverage

)

if (${MOCI_API_OPENGL_MODERN})
    target_compile_definitions(moci_render PUBLIC MOCI_API_OPENGL_MODERN=1)
endif()

if (${MOCI_API_OPENGL_LEGACY})
    target_compile_definitions(moci_render PUBLIC MOCI_API_OPENGL_LEGACY=1)
endif()


set (moci_render_test_src main_test.cpp buffer_test.cpp)
add_executable(moci_render_tests ${moci_render_test_src})
set_target_properties(moci_render_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_render_tests
    PRIVATE
        Moci::RenderApiBase
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_test_src})
catch_discover_tests(moci_render_tests)

add_subdirectory("opengl/common")
add_subdirectory("opengl/es2")
add_subdirectory("opengl/gl4")


set(moci_render_api_sources
    render_command.hpp
    render_command.cpp
    render_factory.cpp
    render_factory.cpp
    renderer_api.cpp
    renderer.hpp
    renderer.cpp

    imgui/imgui_layer.hpp
    imgui/imgui_layer.cpp

    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.cpp

    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp
)

add_library(moci_render_api STATIC ${moci_render_api_sources})
add_library(Moci::RenderApi ALIAS moci_render_api)
# set_target_properties(moci_render_api PROPERTIES UNITY_BUILD OFF)
target_include_directories(moci_render_api PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(moci_render_api
    PUBLIC
        Moci::RenderOpenGL
        Moci::RenderOpenGLES
        Moci::Events
        imgui::imgui
    PRIVATE
        Moci::CodeCoverage
)
