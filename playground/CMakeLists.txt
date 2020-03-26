project(playground)

add_executable(stb_image main.cpp)
target_link_libraries(stb_image 
    PRIVATE 
        Moci::CompilerOptions 
        Moci::CompilerWarnings
        Moci::Moci 
)

add_executable(font_atlas font_atlas.cpp)
target_link_libraries(font_atlas 
    PRIVATE 
        Moci::CompilerOptions 
        Moci::CompilerWarnings
        Moci::Moci 
)

if(APPLE)
    add_executable(glfw_metal glfw_metal.mm)
    set_target_properties(glfw_metal
        PROPERTIES
            MACOSX_BUNDLE YES
            # RESOURCE "src/Main.storyboard"
            # MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )
    target_compile_features(glfw_metal PRIVATE cxx_std_17)
    target_compile_options(glfw_metal PRIVATE "-fobjc-arc" "-Wno-nullability-completeness")
    target_link_libraries(glfw_metal 
        PRIVATE 
            Moci::CompilerOptions 
            Moci::CompilerWarnings
            Moci::Moci 
            "-framework CoreFoundation"
            "-framework Cocoa"
            "-framework Metal"
            "-framework MetalKit"
            "-framework QuartzCore"
    )
endif()

if(WIN32)
    add_executable(win32_gl WIN32 win32_gl.cpp)
    target_compile_features(win32_gl PRIVATE cxx_std_17)
    target_compile_definitions(win32_gl PRIVATE "UNICODE" "_UNICODE")
    target_link_libraries(win32_gl 
        PRIVATE 
            Moci::CompilerOptions 
            Moci::CompilerWarnings
            Moci::Moci 
            user32.lib 
            gdi32.lib
    )
endif()