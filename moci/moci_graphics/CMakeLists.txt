project(moci_graphics)

set(moci_graphics_sources
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

add_library(moci_graphics_lib STATIC ${moci_graphics_sources})
add_library(Moci::Graphics ALIAS moci_graphics_lib)
target_link_libraries(moci_graphics_lib 
    PUBLIC 
        Moci::Application 
        assimp::assimp 
    PRIVATE
       Moci::CodeCoverage
)

set (moci_graphics_test_src
    main_test.cpp
    camera_test.cpp
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
