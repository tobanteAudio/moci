project(moci_widget)

set(moci_widget_sources
    component/component.hpp
    component/component.cpp
    component/slider.hpp
    component/slider.cpp   
    component/widgets.hpp
    component/widgets.cpp   

    layer/component_layer.hpp
    layer/component_layer.cpp

    style/style.hpp
    style/style.cpp
)

add_library(moci_widget_lib STATIC ${moci_widget_sources})
add_library(Moci::Widget ALIAS moci_widget_lib)
target_link_libraries(moci_widget_lib PUBLIC Moci::Graphics)

set (moci_widget_test_src 
    main_test.cpp 
    component/component_test.cpp
    component/slider_test.cpp
    component/widgets_test.cpp 
)
add_executable(moci_widget_tests ${moci_widget_test_src})
target_link_libraries(moci_widget_tests PRIVATE Moci::Widget Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_widget_test_src})
catch_discover_tests(moci_widget_tests)
