project(moci_graphics_extra)

set(moci_graphics_extra_sources
    extra/camera.hpp
    extra/camera.cpp
    extra/light.hpp
    extra/light.cpp
    extra/mesh.hpp
    extra/mesh.cpp
    extra/painter.hpp
    extra/render_queue.hpp
    extra/render_queue.cpp

)

add_library(moci_graphics_extra_lib STATIC ${moci_graphics_extra_sources})
add_library(Moci::GraphicsExtra ALIAS moci_graphics_extra_lib)
set_target_properties(moci_graphics_extra_lib PROPERTIES UNITY_BUILD ON)
target_link_libraries(moci_graphics_extra_lib
    PUBLIC
        Moci::Application
        assimp::assimp
    PRIVATE
       Moci::CodeCoverage
)

set (moci_graphics_extra_test_src
    main_test.cpp
    extra/camera_test.cpp
    extra/mesh_test.cpp
)
add_executable(moci_graphics_extra_tests ${moci_graphics_extra_test_src})
set_target_properties(moci_graphics_extra_tests PROPERTIES UNITY_BUILD ON)
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