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

add_library(moci_widget_lib STATIC ${moci_widget_sources})
add_library(Moci::Widget ALIAS moci_widget_lib)
set_target_properties(moci_widget_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_widget_lib
    PUBLIC
        Moci::GraphicsExtra
    PRIVATE
        Moci::CodeCoverage
)

set (moci_widget_test_src
    main_test.cpp
    # component/button_test.cpp
    component/component_test.cpp
    component/label_test.cpp
    component/slider_test.cpp
)
add_executable(moci_widget_tests ${moci_widget_test_src})
set_target_properties(moci_widget_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_widget_tests
    PRIVATE
        Moci::Widget
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_widget_test_src})
catch_discover_tests(moci_widget_tests)
