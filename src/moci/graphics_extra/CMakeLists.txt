project(moci_graphics_extra)

find_package(assimp REQUIRED)

set(moci_graphics_extra_sources
    camera.hpp
    camera.cpp
    light.hpp
    light.cpp
    mesh.hpp
    mesh.cpp
    painter.hpp
    render_queue.hpp
    render_queue.cpp

)

add_library(moci_graphics_extra ${moci_graphics_extra_sources})
add_library(Moci::GraphicsExtra ALIAS moci_graphics_extra)
target_link_libraries(moci_graphics_extra
    PUBLIC
        Moci::Application
        assimp::assimp
    PRIVATE
       Moci::CodeCoverage
)

set (moci_graphics_extra_test_src
    main_test.cpp
    camera_test.cpp
    mesh_test.cpp
)
add_executable(moci_graphics_extra_tests ${moci_graphics_extra_test_src})
target_link_libraries(moci_graphics_extra_tests
    PRIVATE
        Moci::GraphicsExtra
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_graphics_extra_test_src})
catch_discover_tests(moci_graphics_extra_tests)

# COPY TESTFILES
foreach(_target cube.obj teapot.obj)
execute_process(
    COMMAND
        ${CMAKE_COMMAND} -E
        copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/test_data/${_target}
        ${CMAKE_CURRENT_BINARY_DIR}/moci_test_data/${_target}
)
endforeach()