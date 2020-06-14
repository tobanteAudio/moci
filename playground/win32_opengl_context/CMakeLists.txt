project(win32_opengl_context)

if(WIN32)
    add_executable(win32_gl WIN32 main.cpp)
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