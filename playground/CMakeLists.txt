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

add_executable(glfw_metal glfw_metal.mm)
set_target_properties(glfw_metal
    PROPERTIES
        WIN32 YES
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

