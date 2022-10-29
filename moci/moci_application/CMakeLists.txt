project(moci_application)

set(moci_application_sources
    moci_application.hpp
    moci_application.cpp

    app/application.hpp
    app/application.cpp
    app/entry_point.hpp
    app/window.hpp
    app/window.cpp

    app/glfw/input.hpp
    app/glfw/input.cpp
    app/glfw/window.hpp
    app/glfw/window.cpp
)

add_library(moci_application_lib STATIC ${moci_application_sources})
add_library(Moci::Application ALIAS moci_application_lib)
set_target_properties(moci_application_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_application_lib
    PUBLIC
        Moci::RenderApi
    PRIVATE
        Moci::CodeCoverage
)