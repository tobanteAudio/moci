project(moci_application)

add_library(moci_application)
add_library(Moci::Application ALIAS moci_application)

target_sources(moci_application
    PRIVATE
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

target_link_libraries(moci_application
    PUBLIC
        Moci::RenderApi
        Moci::CodeCoverage
)
