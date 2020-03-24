project(sandbox3D)

set(sandbox3d_sources
    main.cpp
    demo_layer.hpp
    demo_layer.cpp
)
add_executable(${PROJECT_NAME} ${sandbox3d_sources})
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Moci::CompilerOptions 
        Moci::CompilerWarnings
        Moci::Moci
)
