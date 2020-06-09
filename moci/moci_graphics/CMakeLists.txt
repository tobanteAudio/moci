project(moci_graphics)

set(moci_graphics_sources
    camera.hpp
    camera.cpp
    color.hpp
    color.cpp
    font.hpp
    font.cpp
    image.hpp
    image.cpp
    light.hpp
    light.cpp
    mesh.hpp
    mesh.cpp
    render_queue.hpp
    render_queue.cpp

    freetype/library.hpp
    freetype/library.cpp

    obj/file.hpp
    obj/file.cpp

)

add_library(moci_graphics_lib STATIC ${moci_graphics_sources})
add_library(Moci::Graphics ALIAS moci_graphics_lib)
target_link_libraries(moci_graphics_lib PUBLIC Moci::Application assimp::assimp freetype)

set (moci_graphics_test_src
    main_test.cpp

    camera_test.cpp
    color_test.cpp
    font_test.cpp
    image_test.cpp

    freetype/library_test.cpp
    obj/file_test.cpp
)
add_executable(moci_graphics_tests ${moci_graphics_test_src})
target_link_libraries(moci_graphics_tests PRIVATE Moci::Graphics Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_graphics_test_src})
catch_discover_tests(moci_graphics_tests)

# COPY TESTFILES
foreach(_target OpenSans-Bold.ttf cube.obj teapot.obj colors.png)
execute_process(
    COMMAND 
        ${CMAKE_COMMAND} -E 
        copy_if_different 
        ${CMAKE_CURRENT_SOURCE_DIR}/test_data/${_target}
        ${CMAKE_CURRENT_BINARY_DIR}/moci_test_data/${_target}
)
endforeach()