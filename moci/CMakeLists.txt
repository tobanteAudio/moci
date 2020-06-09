project(moci)

add_library(moci_include_path INTERFACE)
add_library(Moci::IncludePath ALIAS moci_include_path)
target_include_directories(moci_include_path INTERFACE ${PROJECT_SOURCE_DIR})

add_subdirectory(moci_core)
add_subdirectory(moci_math)
add_subdirectory(moci_network)
add_subdirectory(moci_system)
add_subdirectory(moci_events)
add_subdirectory(moci_render_base)
add_subdirectory(moci_render_opengl)
add_subdirectory(moci_render_opengles)
add_subdirectory(moci_render_api)
add_subdirectory(moci_application)
add_subdirectory(moci_graphics)

# UI
set(moci_widget_sources
    moci_widget/widgets/component_layer.hpp
    moci_widget/widgets/component_layer.cpp
    moci_widget/widgets/component.hpp
    moci_widget/widgets/component.cpp
    moci_widget/widgets/style.hpp
    moci_widget/widgets/style.cpp
    moci_widget/widgets/widgets.hpp
    moci_widget/widgets/widgets.cpp   
)

add_library(moci_widget_lib STATIC ${moci_widget_sources})
add_library(Moci::Widgets ALIAS moci_widget_lib)
target_link_libraries(moci_widget_lib PUBLIC Moci::Graphics)

set (moci_widget_test_src test_main.cpp moci_widget/widgets/widgets_test.cpp moci_widget/widgets/component_test.cpp)
add_executable(moci_widget_tests ${moci_widget_test_src})
target_link_libraries(moci_widget_tests PRIVATE Moci::Widgets Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_widget_test_src})
catch_discover_tests(moci_widget_tests)

# HEADERS
add_library(${PROJECT_NAME} STATIC moci_header/moci/moci.hpp moci_header/moci/moci.cpp)
add_library(Moci::Moci ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/moci_header /opt/vc/include)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Moci::Core
        Moci::Math
        Moci::Network
        Moci::System
        Moci::Graphics
        Moci::Application
        Moci::Widgets
)

