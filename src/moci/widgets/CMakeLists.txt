project(moci_widget)

set(moci_widget_sources
    component/button.hpp
    component/button.cpp
    component/component.hpp
    component/component.cpp
    component/label.hpp
    component/label.cpp
    component/slider.hpp
    component/slider.cpp

    layer/component_layer.hpp
    layer/component_layer.cpp

    style/style.hpp
    style/style.cpp
)

add_library(moci_widget_lib ${moci_widget_sources})
add_library(Moci::Widget ALIAS moci_widget_lib)
target_link_libraries(moci_widget_lib
    PUBLIC
        Moci::GraphicsExtra
    PRIVATE
        Moci::CodeCoverage
)

set (moci_widget_test_src
    main.test.cpp
    # component/button.test.cpp
    component/component.test.cpp
    component/label.test.cpp
    component/slider.test.cpp
)
add_executable(moci_widget_tests ${moci_widget_test_src})
target_link_libraries(moci_widget_tests
    PRIVATE
        Moci::Widget
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_widget_test_src})
catch_discover_tests(moci_widget_tests)
