project(moci_application)

set(moci_application_sources
    application.hpp
    application.cpp
    entry_point.hpp
    window.hpp
    window.cpp

    glfw/input.hpp
    glfw/input.cpp
    glfw/window.hpp
    glfw/window.cpp
)

add_library(moci_application STATIC ${moci_application_sources})
add_library(Moci::Application ALIAS moci_application)
target_link_libraries(moci_application
    PUBLIC
        Moci::RenderApi
    PRIVATE
        Moci::CodeCoverage
)