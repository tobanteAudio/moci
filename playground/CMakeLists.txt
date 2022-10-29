project(playground)

if(EMSCRIPTEN)
    add_subdirectory(sdl_emscripten)
else()
    add_subdirectory(cxx_stb_image)
    add_subdirectory(freetype_atlas)
endif()


