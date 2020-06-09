project(moci)

add_library(moci_include_path INTERFACE)
add_library(Moci::IncludePath ALIAS moci_include_path)
target_include_directories(moci_include_path INTERFACE ${PROJECT_SOURCE_DIR})

add_subdirectory(moci_core)
add_subdirectory(moci_math)
add_subdirectory(moci_network)
add_subdirectory(moci_system)
add_subdirectory(moci_events)
add_subdirectory(moci_render_base)
add_subdirectory(moci_render_opengl)
add_subdirectory(moci_render_opengles)

# RENDER API
set(moci_render_api_sources
    moci_render_api/render/buffer.cpp
    moci_render_api/render/render_command.hpp
    moci_render_api/render/render_command.cpp
    moci_render_api/render/renderer_api.cpp
    moci_render_api/render/renderer.hpp
    moci_render_api/render/renderer.cpp
    moci_render_api/render/shader.cpp
    moci_render_api/render/texture.cpp
    moci_render_api/render/vertex_array.cpp

    moci_render_api/layer/layer_stack.hpp
    moci_render_api/layer/layer_stack.cpp
    moci_render_api/layer/layer.hpp
    moci_render_api/layer/layer.cpp

    moci_render_api/imgui/imgui_build.cpp
    moci_render_api/imgui/imgui_layer.hpp
    moci_render_api/imgui/imgui_layer.cpp

)

add_library(moci_render_api_lib STATIC ${moci_render_api_sources})
add_library(Moci::RenderApi ALIAS moci_render_api_lib)
target_link_libraries(moci_render_api_lib PUBLIC Moci::RenderOpenGL Moci::RenderOpenGLES Moci::Events Moci::ImGui)


set (moci_render_api_test_src
    test_main.cpp
    
    moci_render_api/layer/layer_stack_test.cpp
)
add_executable(moci_render_api_tests ${moci_render_api_test_src})
target_link_libraries(moci_render_api_tests PRIVATE Moci::RenderApi Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_api_test_src})
catch_discover_tests(moci_render_api_tests)




add_subdirectory(moci_application)


# GRAPHICS 
set(moci_graphics_sources
    moci_graphics/camera.hpp
    moci_graphics/camera.cpp
    moci_graphics/color.hpp
    moci_graphics/color.cpp
    moci_graphics/font.hpp
    moci_graphics/font.cpp
    moci_graphics/image.hpp
    moci_graphics/image.cpp
    moci_graphics/light.hpp
    moci_graphics/light.cpp
    moci_graphics/mesh.hpp
    moci_graphics/mesh.cpp
    moci_graphics/render_queue.hpp
    moci_graphics/render_queue.cpp

    moci_graphics/freetype/library.hpp
    moci_graphics/freetype/library.cpp

    moci_graphics/obj/file.hpp
    moci_graphics/obj/file.cpp

)

add_library(moci_graphics_lib STATIC ${moci_graphics_sources})
add_library(Moci::Graphics ALIAS moci_graphics_lib)
target_link_libraries(moci_graphics_lib PUBLIC Moci::Application assimp::assimp freetype)

set (moci_graphics_test_src
    test_main.cpp

    moci_graphics/camera_test.cpp
    moci_graphics/color_test.cpp
    moci_graphics/font_test.cpp
    moci_graphics/image_test.cpp

    moci_graphics/freetype/library_test.cpp
    moci_graphics/obj/file_test.cpp
)
add_executable(moci_graphics_tests ${moci_graphics_test_src})
target_link_libraries(moci_graphics_tests PRIVATE Moci::Graphics Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_graphics_test_src})
catch_discover_tests(moci_graphics_tests)

# UI
set(moci_widget_sources
    moci_widget/widgets/component_layer.hpp
    moci_widget/widgets/component_layer.cpp
    moci_widget/widgets/component.hpp
    moci_widget/widgets/component.cpp
    moci_widget/widgets/style.hpp
    moci_widget/widgets/style.cpp
    moci_widget/widgets/widgets.hpp
    moci_widget/widgets/widgets.cpp   
)

add_library(moci_widget_lib STATIC ${moci_widget_sources})
add_library(Moci::Widgets ALIAS moci_widget_lib)
target_link_libraries(moci_widget_lib PUBLIC Moci::Graphics)

set (moci_widget_test_src test_main.cpp moci_widget/widgets/widgets_test.cpp moci_widget/widgets/component_test.cpp)
add_executable(moci_widget_tests ${moci_widget_test_src})
target_link_libraries(moci_widget_tests PRIVATE Moci::Widgets Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_widget_test_src})
catch_discover_tests(moci_widget_tests)

# HEADERS
add_library(${PROJECT_NAME} STATIC moci_header/moci/moci.hpp moci_header/moci/moci.cpp)
add_library(Moci::Moci ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/moci_header /opt/vc/include)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Moci::Core
        Moci::Math
        Moci::Network
        Moci::System
        Moci::Graphics
        Moci::Application
        Moci::Widgets
)

# COPY TESTFILES
foreach(_target OpenSans-Bold.ttf cube.obj teapot.obj colors.png)
execute_process(
    COMMAND 
        ${CMAKE_COMMAND} -E 
        copy_if_different 
        ${CMAKE_CURRENT_SOURCE_DIR}/moci_test_data/${_target}
        ${CMAKE_CURRENT_BINARY_DIR}/moci_test_data/${_target}
)
endforeach()