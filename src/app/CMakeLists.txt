project(app)

add_subdirectory(sandbox2D)
add_subdirectory(sandbox3D)
add_subdirectory(sandboxOpenGL)

if(MOCI_BUILD_TOOLS)
    add_subdirectory(tools/model-viewer)
endif(MOCI_BUILD_TOOLS)

if(MOCI_BUILD_PLAYGROUND)
    add_subdirectory(playground/cxx_stb_image)
    add_subdirectory(playground/freetype_atlas)
endif(MOCI_BUILD_PLAYGROUND)