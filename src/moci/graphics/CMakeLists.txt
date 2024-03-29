project(moci_graphics)

find_package(Freetype REQUIRED)

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

add_library(moci_graphics ${moci_graphics_sources})
add_library(Moci::Graphics ALIAS moci_graphics)
target_link_libraries(moci_graphics
    PUBLIC
        Freetype::Freetype
        Moci::Core
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
    PRIVATE
        Moci::CodeCoverage
)

set (moci_graphics_test_src
    main.test.cpp

    color.test.cpp
    font.test.cpp
    image.test.cpp

    freetype/library.test.cpp

    obj/file.test.cpp
)
add_executable(moci_graphics_tests ${moci_graphics_test_src})
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
