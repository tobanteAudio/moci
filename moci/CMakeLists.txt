project(moci)

add_library(moci_include_path INTERFACE)
add_library(Moci::IncludePath ALIAS moci_include_path)
target_include_directories(moci_include_path INTERFACE ${PROJECT_SOURCE_DIR})

add_subdirectory(moci_core)
add_subdirectory(moci_math)
add_subdirectory(moci_network)


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
target_link_libraries(moci_render_base_lib PUBLIC Moci::Math glfw ${GLFW_LIBRARIES})
target_include_directories(moci_render_base_lib PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include)


set (moci_render_base_test_src test_main.cpp moci_render_base/render/buffer_test.cpp)
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