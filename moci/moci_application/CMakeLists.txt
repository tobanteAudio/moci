project(moci_application)

set(moci_application_sources
    moci_application.hpp
    moci_application.cpp

    src/application.hpp
    src/application.cpp
    src/entry_point.hpp
    src/window.hpp
    src/window.cpp
    
    src/glfw/input.hpp
    src/glfw/input.cpp
    src/glfw/window.hpp
    src/glfw/window.cpp
)

add_library(moci_application_lib STATIC ${moci_application_sources})
add_library(Moci::Application ALIAS moci_application_lib)
target_link_libraries(moci_application_lib PUBLIC Moci::RenderApi)