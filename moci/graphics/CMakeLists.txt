project(moci_graphics)

set(moci_graphics_sources
    color.hpp
    color.cpp
    font.hpp
    font.cpp
    image.hpp
    image.cpp

    freetype/library.hpp
    freetype/library.cpp

    obj/file.hpp
    obj/file.cpp

)

add_library(moci_graphics STATIC ${moci_graphics_sources})
add_library(Moci::Graphics ALIAS moci_graphics)
set_target_properties(moci_graphics PROPERTIES UNITY_BUILD ON)
target_link_libraries(moci_graphics
    PUBLIC
        Moci::Math
        Freetype::Freetype
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
    PRIVATE
        Moci::CodeCoverage
)

set (moci_graphics_test_src
    main_test.cpp

    color_test.cpp
    font_test.cpp
    image_test.cpp

    freetype/library_test.cpp

    obj/file_test.cpp
)
add_executable(moci_graphics_tests ${moci_graphics_test_src})
set_target_properties(moci_graphics_tests PROPERTIES UNITY_BUILD ON)
target_link_libraries(moci_graphics_tests
    PRIVATE
        Moci::Graphics
        Moci::CodeCoverage
        Catch2::Catch2
)
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