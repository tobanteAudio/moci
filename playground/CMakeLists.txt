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
