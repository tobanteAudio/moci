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

add_subdirectory(glfw_metal)

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