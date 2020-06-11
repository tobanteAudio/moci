project(moci_render_api)

set(moci_render_api_sources
    render/buffer.cpp
    render/frame_buffer.cpp
    render/frame_buffer.cpp
    render/render_command.hpp
    render/render_command.cpp
    render/renderer_api.cpp
    render/renderer.hpp
    render/renderer.cpp
    render/shader.cpp
    render/texture.cpp
    render/vertex_array.cpp

    layer/layer_stack.hpp
    layer/layer_stack.cpp
    layer/layer.hpp
    layer/layer.cpp

    imgui/imgui_build.cpp
    imgui/imgui_layer.hpp
    imgui/imgui_layer.cpp

)

add_library(moci_render_api_lib STATIC ${moci_render_api_sources})
add_library(Moci::RenderApi ALIAS moci_render_api_lib)
target_link_libraries(moci_render_api_lib PUBLIC Moci::RenderOpenGL Moci::RenderOpenGLES Moci::Events Moci::ImGui)

set (moci_render_api_test_src main_test.cpp layer/layer_stack_test.cpp)
add_executable(moci_render_api_tests ${moci_render_api_test_src})
target_link_libraries(moci_render_api_tests PRIVATE Moci::RenderApi Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_api_test_src})
catch_discover_tests(moci_render_api_tests)

