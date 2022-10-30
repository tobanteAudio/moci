project(moci)

add_library(moci_include_path INTERFACE)
add_library(Moci::IncludePath ALIAS moci_include_path)
target_include_directories(moci_include_path INTERFACE ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})

add_subdirectory(moci_external)

add_subdirectory("core")
add_subdirectory("math")
add_subdirectory("network")
add_subdirectory("system")
add_subdirectory("events")
add_subdirectory("graphics")
add_subdirectory("render")
add_subdirectory("application")

add_subdirectory(moci_graphics_extra)
add_subdirectory(moci_widget)
add_subdirectory(moci_header)