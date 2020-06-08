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


set (moci_system_test_src
    test_main.cpp
    
    moci_system/system/info_test.cpp
)
add_executable(moci_system_tests ${moci_system_test_src})
target_link_libraries(moci_system_tests PRIVATE Moci::System Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_system_test_src})
catch_discover_tests(moci_system_tests)





# EVENTS
set(moci_events_sources
    moci_events/events/application_event.hpp
    moci_events/events/event.hpp
    moci_events/events/key_event.hpp
    moci_events/events/mouse_event.hpp
    moci_events/input.hpp
    moci_events/input.cpp
    moci_events/key_codes.hpp
    moci_events/mouse_button_codes.hpp
    moci_events/timestep.hpp
)

add_library(moci_events_lib STATIC ${moci_events_sources})
add_library(Moci::Events ALIAS moci_events_lib)
target_link_libraries(moci_events_lib PUBLIC Moci::Core)


set (moci_events_test_src
    test_main.cpp
    
    moci_events/events/application_event_test.cpp
    moci_events/events/key_event_test.cpp
    moci_events/events/mouse_event_test.cpp
)
add_executable(moci_events_tests ${moci_events_test_src})
target_link_libraries(moci_events_tests PRIVATE Moci::Events Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_events_test_src})
catch_discover_tests(moci_events_tests)







# RENDER BASE
set(moci_render_base_sources
    moci_render_base/render/api.hpp
    moci_render_base/render/buffer.hpp
    moci_render_base/render/buffer.cpp
    moci_render_base/render/graphics_context.hpp
    moci_render_base/render/graphics_context.cpp
    moci_render_base/render/shader.hpp
    moci_render_base/render/texture.hpp
    moci_render_base/render/vertex_array.hpp

)

add_library(moci_render_base_lib STATIC ${moci_render_base_sources})
add_library(Moci::RenderApiBase ALIAS moci_render_base_lib)
target_link_libraries(moci_render_base_lib PUBLIC Moci::Math)


set (moci_render_base_test_src
    test_main.cpp

    moci_render_base/render/buffer_test.cpp
)
add_executable(moci_render_base_tests ${moci_render_base_test_src})
target_link_libraries(moci_render_base_tests PRIVATE Moci::RenderApiBase Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_base_test_src})
catch_discover_tests(moci_render_base_tests)




# RENDER OPENGLES
set(moci_render_opengles_sources
    moci_render_opengles/moci_render_opengles.hpp
    moci_render_opengles/buffer.hpp
    moci_render_opengles/buffer.cpp
    moci_render_opengles/graphics_context.hpp
    moci_render_opengles/graphics_context.cpp
    moci_render_opengles/es2.hpp
    moci_render_opengles/es2.cpp
    moci_render_opengles/renderer_api.hpp
    moci_render_opengles/renderer_api.cpp
    moci_render_opengles/shader.hpp
    moci_render_opengles/shader.cpp
    moci_render_opengles/texture.hpp
    moci_render_opengles/texture.cpp
    moci_render_opengles/vertex_array.hpp
    moci_render_opengles/vertex_array.cpp

)

add_library(moci_render_opengles_lib STATIC ${moci_render_opengles_sources})
add_library(Moci::RenderOpenGLES ALIAS moci_render_opengles_lib)
target_link_libraries(moci_render_opengles_lib 
    PUBLIC 
        Moci::RenderApiBase
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
        freetype
        glfw 
        ${GLFW_LIBRARIES}
)

if (${MOCI_API_OPENGL_MODERN})
    target_compile_definitions(moci_render_opengles_lib PUBLIC MOCI_API_OPENGL_MODERN=1)
endif()

if (${MOCI_API_OPENGL_LEGACY})
    target_compile_definitions(moci_render_opengles_lib PUBLIC MOCI_API_OPENGL_LEGACY=1)
endif()

if (${MOCI_API_OPENGL_ES})
    # old
    # target_link_libraries(moci_render_opengles_lib PUBLIC -lGLESv2)
    # raspberry pi 3
    target_link_libraries(moci_render_opengles_lib PUBLIC /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2)
    # desktop linux
    # target_link_libraries(moci_render_opengles_lib PUBLIC /usr/lib/x86_64-linux-gnu/libGLESv2.so)
    
    target_compile_definitions(moci_render_opengles_lib PUBLIC MOCI_API_OPENGL_ES=1)
else()
    target_link_libraries(moci_render_opengles_lib PUBLIC libglew_static)
endif()



# set (moci_render_opengles_test_src
#     test_main.cpp

#     moci_render_opengles/render/buffer_test.cpp
# )
# add_executable(moci_render_opengles_tests ${moci_render_opengles_test_src})
# target_link_libraries(moci_render_opengles_tests PRIVATE Moci::RenderOpenGLES Catch2::Catch2)
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_opengles_test_src})
# catch_discover_tests(moci_render_opengles_tests)





# RENDER OPENGL
set(moci_render_opengl_sources
    moci_render_opengl/moci_render_opengl.hpp
    moci_render_opengl/buffer.hpp
    moci_render_opengl/buffer.cpp
    moci_render_opengl/graphics_context.hpp
    moci_render_opengl/graphics_context.cpp
    moci_render_opengl/gl4.hpp
    moci_render_opengl/gl4.cpp
    moci_render_opengl/renderer_api.hpp
    moci_render_opengl/renderer_api.cpp
    moci_render_opengl/shader.hpp
    moci_render_opengl/shader.cpp
    moci_render_opengl/texture.hpp
    moci_render_opengl/texture.cpp
    moci_render_opengl/vertex_array.hpp
    moci_render_opengl/vertex_array.cpp

)

add_library(moci_render_opengl_lib STATIC ${moci_render_opengl_sources})
add_library(Moci::RenderOpenGL ALIAS moci_render_opengl_lib)
target_link_libraries(moci_render_opengl_lib 
    PUBLIC 
        Moci::RenderApiBase
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
        freetype
        glfw 
        ${GLFW_LIBRARIES}
)

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
#     test_main.cpp

#     moci_render_opengl/render/buffer_test.cpp
# )
# add_executable(moci_render_opengl_tests ${moci_render_opengl_test_src})
# target_link_libraries(moci_render_opengl_tests PRIVATE Moci::RenderOpenGL Catch2::Catch2)
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_opengl_test_src})
# catch_discover_tests(moci_render_opengl_tests)




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

)

add_library(moci_render_api_lib STATIC ${moci_render_api_sources})
add_library(Moci::RenderApi ALIAS moci_render_api_lib)
target_link_libraries(moci_render_api_lib PUBLIC Moci::RenderOpenGL Moci::RenderOpenGLES Moci::Events Moci::ImGui)


# set (moci_render_api_test_src
#     test_main.cpp
    
#     moci_render_api/layer_stack_test.cpp
# )
# add_executable(moci_render_api_tests ${moci_render_api_test_src})
# target_link_libraries(moci_render_api_tests PRIVATE Moci::RenderApi Catch2::Catch2)
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_api_test_src})
# catch_discover_tests(moci_render_api_tests)



# GRAPHICS 
set(moci_graphics_sources
    moci_graphics/imgui/imgui_build.cpp
    moci_graphics/imgui/imgui_layer.hpp
    moci_graphics/imgui/imgui_layer.cpp
        
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
target_link_libraries(moci_graphics_lib 
    PUBLIC 
        Moci::RenderApi 
        assimp::assimp
)


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



# APPLICATION
set(moci_application_sources
    moci_application/application.hpp
    moci_application/application.cpp
    moci_application/entry_point.hpp
    moci_application/layer_stack.hpp
    moci_application/layer_stack.cpp
    moci_application/layer.hpp
    moci_application/layer.cpp
    moci_application/window.hpp
    moci_application/window.cpp
    
    moci_application/glfw/input.hpp
    moci_application/glfw/input.cpp
    moci_application/glfw/window.hpp
    moci_application/glfw/window.cpp
)

add_library(moci_application_lib STATIC ${moci_application_sources})
add_library(Moci::Application ALIAS moci_application_lib)
target_link_libraries(moci_application_lib PUBLIC Moci::Graphics)


set (moci_application_test_src
    test_main.cpp
    
    moci_application/layer_stack_test.cpp
)
add_executable(moci_application_tests ${moci_application_test_src})
target_link_libraries(moci_application_tests PRIVATE Moci::Application Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_application_test_src})
catch_discover_tests(moci_application_tests)

# ORIGINAL
set(moci_sources
    moci/moci.hpp
    moci/moci.cpp
    
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
        Moci::Graphics
        Moci::Application
)


# tests
set (moci_test_src
    test_main.cpp

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