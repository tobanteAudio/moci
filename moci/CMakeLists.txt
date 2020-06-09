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
add_subdirectory(moci_widget)

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
        Moci::Widget
)

