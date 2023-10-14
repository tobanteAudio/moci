project(moci)

add_library(moci_include_path INTERFACE)
add_library(Moci::IncludePath ALIAS moci_include_path)
target_include_directories(moci_include_path INTERFACE ${PROJECT_SOURCE_DIR})

add_subdirectory("moci/external")
add_subdirectory("moci/core")
add_subdirectory("moci/events")
add_subdirectory("moci/graphics")
add_subdirectory("moci/render")
add_subdirectory("moci/application")
add_subdirectory("moci/graphics_extra")
add_subdirectory("moci/widgets")

add_subdirectory("app")
