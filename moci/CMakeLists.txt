project(moci)

add_library(moci_include_path INTERFACE)
add_library(Moci::IncludePath ALIAS moci_include_path)
target_include_directories(moci_include_path INTERFACE ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})

add_subdirectory(moci_external)

add_subdirectory("core")
add_subdirectory("math")

add_subdirectory(moci_network)
add_subdirectory(moci_system)
add_subdirectory(moci_events)
add_subdirectory(moci_graphics_base)
add_subdirectory(moci_render_base)
add_subdirectory(moci_render_opengl_common)
add_subdirectory(moci_render_opengl)
add_subdirectory(moci_render_opengles)
add_subdirectory(moci_render_api)
add_subdirectory(moci_application)
add_subdirectory(moci_graphics_extra)
add_subdirectory(moci_widget)
add_subdirectory(moci_header)