project(moci_graphics_base)

set(moci_graphics_base_sources
    base/color.hpp
    base/color.cpp
    base/font.hpp
    base/font.cpp
    base/image.hpp
    base/image.cpp

    base/freetype/library.hpp
    base/freetype/library.cpp

    base/obj/file.hpp
    base/obj/file.cpp

)

add_library(moci_graphics_base_lib STATIC ${moci_graphics_base_sources})
add_library(Moci::GraphicsBase ALIAS moci_graphics_base_lib)
# set_target_properties(moci_graphics_base_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_graphics_base_lib
    PUBLIC
        Moci::Math
        Freetype::Freetype
        Moci::StbImage
        Moci::StbImageWrite
        Moci::StbImageResize
    PRIVATE
        Moci::CodeCoverage
)

set (moci_graphics_base_test_src
    main_test.cpp

    base/color_test.cpp
    base/font_test.cpp
    base/image_test.cpp

    base/freetype/library_test.cpp

    base/obj/file_test.cpp
)
add_executable(moci_graphics_base_tests ${moci_graphics_base_test_src})
set_target_properties(moci_graphics_base_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_graphics_base_tests
    PRIVATE
        Moci::GraphicsBase
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_graphics_base_test_src})
catch_discover_tests(moci_graphics_base_tests)

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