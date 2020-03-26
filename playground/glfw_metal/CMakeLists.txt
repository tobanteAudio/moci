project(glfw_metal)

if(APPLE)
    set(glfw_metal_sources
        glfw_metal.mm
        mtlpp.hpp
        mtlpp.mm
    )
    foreach(source IN LISTS glfw_metal_sources)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()
    add_executable(${PROJECT_NAME} ${glfw_metal_sources})
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            MACOSX_BUNDLE YES
            # RESOURCE "src/Main.storyboard"
            # MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
    target_compile_options(${PROJECT_NAME} PRIVATE "-mmacosx-version-min=10.12")
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
            Moci::CompilerOptions 
            Moci::CompilerWarnings
            Moci::Moci 
            "-fobjc-link-runtime"
            "-framework CoreFoundation"
            "-framework Cocoa"
            "-framework Metal"
            "-framework MetalKit"
            "-framework QuartzCore"
    )
endif()