project(moci_render_base)

set(moci_render_base_sources
    moci_render_base.hpp
    moci_render_base.cpp
    
    render/api.hpp
    render/buffer.hpp
    render/buffer.cpp
    render/graphics_context.hpp
    render/graphics_context.cpp
    render/shader.hpp
    render/texture.hpp
    render/vertex_array.hpp

)

add_library(moci_render_base_lib STATIC ${moci_render_base_sources})
add_library(Moci::RenderApiBase ALIAS moci_render_base_lib)

target_include_directories(moci_render_base_lib PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include)
target_link_libraries(moci_render_base_lib 
    PUBLIC 
        Moci::Math 
        glfw 
        ${GLFW_LIBRARIES}
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
)


set (moci_render_base_test_src main_test.cpp render/buffer_test.cpp)
add_executable(moci_render_base_tests ${moci_render_base_test_src})
target_link_libraries(moci_render_base_tests PRIVATE Moci::RenderApiBase Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_render_base_test_src})
catch_discover_tests(moci_render_base_tests)
