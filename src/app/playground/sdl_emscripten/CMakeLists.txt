cmake_minimum_required(VERSION 3.21)
project(sdl_emscripten)

find_package(imgui REQUIRED)

add_executable(sdl_emscripten
    main.cpp

    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_sdl.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_sdl.cpp
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp
)

target_include_directories(sdl_emscripten PRIVATE ${CMAKE_BINARY_DIR})
target_compile_definitions(sdl_emscripten PRIVATE IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
target_link_libraries(sdl_emscripten PRIVATE imgui::imgui)

target_compile_options(sdl_emscripten
    PRIVATE
        "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
        "SHELL:-s USE_BOOST_HEADERS=1"
        "SHELL:-s USE_SDL=2"
)
target_link_options(sdl_emscripten
    PRIVATE
        "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
        "SHELL:-s USE_BOOST_HEADERS=1"
        "SHELL:-s USE_SDL=2"
        "SHELL:-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1"
)