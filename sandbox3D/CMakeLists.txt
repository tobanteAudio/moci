project(sandbox3D)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Moci::CompilerOptions 
        Moci::CompilerWarnings
        Moci::Moci 
)
