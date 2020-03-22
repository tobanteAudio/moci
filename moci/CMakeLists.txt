project(moci)

set(moci_sources
    moci/moci.hpp
    moci/moci.cpp
    
    moci/core/algorithm.hpp
    moci/core/logging.hpp
    moci/core/logging.cpp
    moci/core/preprocessor.hpp
    moci/core/property_map.hpp
    moci/core/property_map.cpp
    moci/core/scope_close.hpp
    moci/core/scope_close.cpp
    moci/core/strings.hpp

    moci/debug/instrumentor.hpp
    
    moci/app/application.hpp
    moci/app/application.cpp
    moci/app/entry_point.hpp
    moci/app/events/application_event.hpp
    moci/app/events/event.hpp
    moci/app/events/key_event.hpp
    moci/app/events/mouse_event.hpp
    moci/app/input.hpp
    moci/app/input.cpp
    moci/app/key_codes.hpp
    moci/app/layer_stack.hpp
    moci/app/layer_stack.cpp
    moci/app/layer.hpp
    moci/app/layer.cpp
    moci/app/mouse_button_codes.hpp
    moci/app/timestep.hpp
    moci/app/window.hpp
    moci/app/window.cpp
    
    moci/app/glfw/input.hpp
    moci/app/glfw/input.cpp
    moci/app/glfw/window.hpp
    moci/app/glfw/window.cpp

    moci/geometry/line.hpp
    moci/geometry/rectangle.hpp
    moci/geometry/point.hpp

    moci/network/datagram.hpp
    moci/network/datagram.cpp

    moci/network/unix/datagram.hpp
    moci/network/unix/datagram.cpp
    
    moci/network/windows/datagram.hpp
    moci/network/windows/datagram.cpp

    moci/render/imgui/imgui_build.cpp
    moci/render/imgui/imgui_layer.hpp
    moci/render/imgui/imgui_layer.cpp
        
    moci/render/buffer.hpp
    moci/render/buffer.cpp
    moci/render/camera.hpp
    moci/render/camera.cpp
    moci/render/color.hpp
    moci/render/color.cpp
    moci/render/font.hpp
    moci/render/font.cpp
    moci/render/graphics_context.hpp
    moci/render/graphics_context.cpp
    moci/render/render_command.hpp
    moci/render/render_command.cpp
    moci/render/render_queue.hpp
    moci/render/render_queue.cpp
    moci/render/renderer_api.hpp
    moci/render/renderer_api.cpp
    moci/render/renderer.hpp
    moci/render/renderer.cpp
    moci/render/shader.hpp
    moci/render/shader.cpp
    moci/render/texture.hpp
    moci/render/texture.cpp
    moci/render/vertex_array.hpp
    moci/render/vertex_array.cpp

    moci/render/freetype/library.hpp
    moci/render/freetype/library.cpp

    moci/render/obj/file.hpp
    moci/render/obj/file.cpp

    moci/render/opengl/gl4/buffer.hpp
    moci/render/opengl/gl4/buffer.cpp
    moci/render/opengl/gl4/graphics_context.hpp
    moci/render/opengl/gl4/graphics_context.cpp
    moci/render/opengl/gl4/gl4.hpp
    moci/render/opengl/gl4/gl4.cpp
    moci/render/opengl/gl4/renderer_api.hpp
    moci/render/opengl/gl4/renderer_api.cpp
    moci/render/opengl/gl4/shader.hpp
    moci/render/opengl/gl4/shader.cpp
    moci/render/opengl/gl4/texture.hpp
    moci/render/opengl/gl4/texture.cpp
    moci/render/opengl/gl4/vertex_array.hpp
    moci/render/opengl/gl4/vertex_array.cpp

    moci/render/opengl/es2/buffer.hpp
    moci/render/opengl/es2/buffer.cpp
    moci/render/opengl/es2/graphics_context.hpp
    moci/render/opengl/es2/graphics_context.cpp
    moci/render/opengl/es2/es2.hpp
    moci/render/opengl/es2/es2.cpp
    moci/render/opengl/es2/renderer_api.hpp
    moci/render/opengl/es2/renderer_api.cpp
    moci/render/opengl/es2/shader.hpp
    moci/render/opengl/es2/shader.cpp
    moci/render/opengl/es2/texture.hpp
    moci/render/opengl/es2/texture.cpp
    moci/render/opengl/es2/vertex_array.hpp
    moci/render/opengl/es2/vertex_array.cpp

    moci/ui/component_layer.hpp
    moci/ui/component_layer.cpp
    moci/ui/component.hpp
    moci/ui/component.cpp
    moci/ui/style.hpp
    moci/ui/style.cpp
    moci/ui/widgets.hpp
    moci/ui/widgets.cpp

    moci/system/info.hpp
    moci/system/info.cpp
    
    moci/system/apple/info.hpp
    moci/system/apple/info.cpp

    moci/system/linux/info.hpp
    moci/system/linux/info.cpp

    moci/system/windows/info.hpp
    moci/system/windows/info.cpp
    
)

add_library(${PROJECT_NAME} STATIC ${moci_sources})
add_library(Moci::Moci ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

target_compile_definitions(${PROJECT_NAME} PUBLIC 
    NDEBUG=1
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include
    ${CMAKE_SOURCE_DIR}/3rd_party/glm
    .
    /opt/vc/include
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Moci::Threads
        Moci::ImGui
        Moci::StbImage
        spdlog::spdlog
        glfw 
        ${GLFW_LIBRARIES}
        glm::glm
        GSL
        freetype
    PRIVATE 
        Moci::CodeCoverage
        Moci::CompilerOptions
        Moci::CompilerWarnings
)

if (${MOCI_API_OPENGL_MODERN})
    target_compile_definitions(${PROJECT_NAME} PUBLIC MOCI_API_OPENGL_MODERN=1)
endif()

if (${MOCI_API_OPENGL_LEGACY})
    target_compile_definitions(${PROJECT_NAME} PUBLIC MOCI_API_OPENGL_LEGACY=1)
endif()

if (${MOCI_API_OPENGL_ES})
    # old
    # target_link_libraries(${PROJECT_NAME} PUBLIC -lGLESv2)
    # raspberry pi 3
    # target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    
    target_compile_definitions(${PROJECT_NAME} PUBLIC MOCI_API_OPENGL_ES=1)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC libglew_static)
endif()

# tests
set (moci_test_src
    test_main.cpp
    
    moci/core/algorithm_test.cpp
    moci/core/property_map_test.cpp
    moci/core/scope_close_test.cpp
    moci/core/strings_test.cpp

    moci/app/layer_stack_test.cpp
    moci/app/events/application_event_test.cpp
    moci/app/events/key_event_test.cpp
    moci/app/events/mouse_event_test.cpp

    moci/geometry/line_test.cpp
    moci/geometry/rectangle_test.cpp
    moci/geometry/point_test.cpp

    moci/network/datagram_test.cpp

    moci/render/buffer_test.cpp
    moci/render/camera_test.cpp
    moci/render/color_test.cpp
    moci/render/font_test.cpp

    moci/render/freetype/library_test.cpp
    moci/render/obj/file_test.cpp

    moci/ui/component_test.cpp
    moci/ui/widgets_test.cpp

    moci/system/info_test.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_test_src})
add_executable(moci_tests ${moci_test_src})
target_link_libraries(moci_tests 
    PRIVATE 
        Moci::Moci 
        Moci::CodeCoverage
        Moci::CompilerOptions
        Moci::CompilerWarnings
        Catch2::Catch2
)
catch_discover_tests(moci_tests)

# copy testfiles
foreach(_target OpenSans-Bold.ttf cube.obj teapot.obj)
execute_process(
    COMMAND 
        ${CMAKE_COMMAND} -E 
        copy_if_different 
        ${CMAKE_CURRENT_SOURCE_DIR}/test_data/${_target}
        ${CMAKE_CURRENT_BINARY_DIR}/test_data/${_target}
)
endforeach()