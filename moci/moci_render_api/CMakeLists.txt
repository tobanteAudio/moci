project(moci_render_api)

set(moci_render_api_sources
    render/render_command.hpp
    render/render_command.cpp
    render/render_factory.cpp
    render/render_factory.cpp
    render/renderer_api.cpp
    render/renderer.hpp
    render/renderer.cpp

    imgui/imgui_build.cpp
    imgui/imgui_layer.hpp
    imgui/imgui_layer.cpp

)

add_library(moci_render_api_lib STATIC ${moci_render_api_sources})
add_library(Moci::RenderApi ALIAS moci_render_api_lib)
# set_target_properties(moci_render_api_lib PROPERTIES UNITY_BUILD ON)
target_link_libraries(moci_render_api_lib
    PUBLIC
        Moci::RenderOpenGL
        Moci::RenderOpenGLES
        Moci::Events
        Moci::ImGui
    PRIVATE
        Moci::CodeCoverage
)

