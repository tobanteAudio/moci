project(sandboxOpenGL)

set(sandboxOpenGL_sources
    main.cpp
    opengl_layer.hpp
    opengl_layer.cpp
)
add_executable(${PROJECT_NAME} ${sandboxOpenGL_sources})
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Moci::CompilerOptions 
        Moci::CompilerWarnings
        Moci::Moci
)
