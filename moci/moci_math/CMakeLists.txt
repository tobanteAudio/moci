project(moci_math)

# MATH
set(moci_math_sources
    geometry/line.hpp
    geometry/rectangle.hpp
    geometry/rectangle.cpp
    geometry/point.hpp
)

add_library(moci_math_lib STATIC ${moci_math_sources})
add_library(Moci::Math ALIAS moci_math_lib)
target_link_libraries(moci_math_lib PUBLIC Moci::Core glm::glm)
target_include_directories(moci_math_lib PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/glm)

set (moci_math_test_src
    main_test.cpp
    
    geometry/line_test.cpp
    geometry/rectangle_test.cpp
    geometry/point_test.cpp
)
add_executable(moci_math_tests ${moci_math_test_src})
target_link_libraries(moci_math_tests PRIVATE Moci::Math Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_math_test_src})
catch_discover_tests(moci_math_tests)