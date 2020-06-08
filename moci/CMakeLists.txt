project(moci)

# CORE
set(moci_core_sources
    moci_core/benchmark/benchmark.hpp
    moci_core/benchmark/profile.hpp

    moci_core/core/algorithm.hpp
    moci_core/core/logging.hpp
    moci_core/core/logging.cpp
    moci_core/core/memory.hpp
    moci_core/core/preprocessor.hpp
    moci_core/core/property_map.hpp
    moci_core/core/property_map.cpp
    moci_core/core/scope_guard.hpp
    moci_core/core/scope_guard.cpp
    moci_core/core/strings.hpp
    moci_core/core/vector.hpp
)

add_library(moci_core_lib STATIC ${moci_core_sources})
add_library(Moci::Core ALIAS moci_core_lib)
target_compile_definitions(moci_core_lib PUBLIC NDEBUG=1)
target_include_directories(moci_core_lib PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(moci_core_lib 
    PUBLIC 
        Moci::CodeCoverage
        Moci::CompilerOptions
        Moci::CompilerWarnings
        spdlog::spdlog
        GSL
)

foreach(source IN LISTS moci_sources)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set (moci_core_test_src
    test_main.cpp
    
    moci_core/core/algorithm_test.cpp
    moci_core/core/property_map_test.cpp
    moci_core/core/scope_guard_test.cpp
    moci_core/core/strings_test.cpp
)
add_executable(moci_core_tests ${moci_core_test_src})
target_link_libraries(moci_core_tests PRIVATE Moci::Core Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_core_test_src})
catch_discover_tests(moci_core_tests)





# MATH
set(moci_math_sources
    moci_math/geometry/line.hpp
    moci_math/geometry/rectangle.hpp
    moci_math/geometry/rectangle.cpp
    moci_math/geometry/point.hpp
)

add_library(moci_math_lib STATIC ${moci_math_sources})
add_library(Moci::Math ALIAS moci_math_lib)
target_link_libraries(moci_math_lib PUBLIC Moci::Core glm::glm)

foreach(source IN LISTS moci_sources)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set (moci_math_test_src
    test_main.cpp
    
    moci_math/geometry/line_test.cpp
    moci_math/geometry/rectangle_test.cpp
    moci_math/geometry/point_test.cpp
)
add_executable(moci_math_tests ${moci_math_test_src})
target_link_libraries(moci_math_tests PRIVATE Moci::Math Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_math_test_src})
catch_discover_tests(moci_math_tests)



# NETWORK
set(moci_network_sources
    moci_network/datagram.hpp
    moci_network/datagram.cpp

    moci_network/unix/datagram.hpp
    moci_network/unix/datagram.cpp

    moci_network/windows/datagram.hpp
    moci_network/windows/datagram.cpp
)

add_library(moci_network_lib STATIC ${moci_network_sources})
add_library(Moci::Network ALIAS moci_network_lib)
target_link_libraries(moci_network_lib PUBLIC Moci::Core Moci::Threads)

foreach(source IN LISTS moci_sources)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set (moci_network_test_src
    test_main.cpp
    
    moci_network/datagram_test.cpp
)
add_executable(moci_network_tests ${moci_network_test_src})
target_link_libraries(moci_network_tests PRIVATE Moci::Network Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_network_test_src})
catch_discover_tests(moci_network_tests)



# SYSTEM
set(moci_system_sources
    moci_system/system/info.hpp
    moci_system/system/info.cpp
    moci_system/system/apple/info.hpp
    moci_system/system/apple/info.cpp
    moci_system/system/linux/info.hpp
    moci_system/system/linux/info.cpp
    moci_system/system/windows/info.hpp
    moci_system/system/windows/info.cpp
)

add_library(moci_system_lib STATIC ${moci_system_sources})
add_library(Moci::System ALIAS moci_system_lib)
target_link_libraries(moci_system_lib PUBLIC Moci::Core Moci::Threads)

foreach(source IN LISTS moci_sources)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set (moci_system_test_src
    test_main.cpp
    
    moci_system/system/info_test.cpp
)
add_executable(moci_system_tests ${moci_system_test_src})
target_link_libraries(moci_system_tests PRIVATE Moci::System Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_system_test_src})
catch_discover_tests(moci_system_tests)



# GRAPHICS API BASE
set(moci_graphics_api_base_sources
    moci_graphics_api_base/render/api.hpp
    moci_graphics_api_base/render/buffer.hpp
    moci_graphics_api_base/render/buffer.cpp
)

add_library(moci_graphics_api_base_lib STATIC ${moci_graphics_api_base_sources})
add_library(Moci::GraphicsApiBase ALIAS moci_graphics_api_base_lib)
target_link_libraries(moci_graphics_api_base_lib PUBLIC Moci::Core Moci::Threads)

foreach(source IN LISTS moci_sources)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set (moci_graphics_api_base_test_src
    test_main.cpp

    moci_graphics_api_base/render/buffer_test.cpp
)
add_executable(moci_graphics_api_base_tests ${moci_graphics_api_base_test_src})
target_link_libraries(moci_graphics_api_base_tests PRIVATE Moci::GraphicsApiBase Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_graphics_api_base_test_src})
catch_discover_tests(moci_graphics_api_base_tests)



# ORIGINAL
set(moci_sources
    moci/moci.hpp
    moci/moci.cpp
    
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


    moci/render/imgui/imgui_build.cpp
    moci/render/imgui/imgui_layer.hpp
    moci/render/imgui/imgui_layer.cpp
        
    moci/render/buffer.cpp
    moci/render/camera.hpp
    moci/render/camera.cpp
    moci/render/color.hpp
    moci/render/color.cpp
    moci/render/font.hpp
    moci/render/font.cpp
    moci/render/graphics_context.hpp
    moci/render/graphics_context.cpp
    moci/render/image.hpp
    moci/render/image.cpp
    moci/render/light.hpp
    moci/render/light.cpp
    moci/render/mesh.hpp
    moci/render/mesh.cpp
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

    
)

add_library(${PROJECT_NAME} STATIC ${moci_sources})
add_library(Moci::Moci ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
foreach(source IN LISTS moci_sources)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include
    ${CMAKE_SOURCE_DIR}/3rd_party/glm
    .
    /opt/vc/include
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Moci::Core
        Moci::Math
        Moci::Network
        Moci::System
        Moci::GraphicsApiBase
        Moci::ImGui
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
        glfw 
        ${GLFW_LIBRARIES}
        freetype
        assimp::assimp
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
    target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    # target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    
    target_compile_definitions(${PROJECT_NAME} PUBLIC MOCI_API_OPENGL_ES=1)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC libglew_static)
endif()

# tests
set (moci_test_src
    test_main.cpp

    moci/app/layer_stack_test.cpp
    moci/app/events/application_event_test.cpp
    moci/app/events/key_event_test.cpp
    moci/app/events/mouse_event_test.cpp

    moci/render/camera_test.cpp
    moci/render/color_test.cpp
    moci/render/font_test.cpp
    moci/render/image_test.cpp

    moci/render/freetype/library_test.cpp
    moci/render/obj/file_test.cpp

    moci/ui/component_test.cpp
    moci/ui/widgets_test.cpp

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
foreach(_target OpenSans-Bold.ttf cube.obj teapot.obj colors.png)
execute_process(
    COMMAND 
        ${CMAKE_COMMAND} -E 
        copy_if_different 
        ${CMAKE_CURRENT_SOURCE_DIR}/test_data/${_target}
        ${CMAKE_CURRENT_BINARY_DIR}/test_data/${_target}
)
endforeach()