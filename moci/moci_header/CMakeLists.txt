project(moci_header)

add_library(${PROJECT_NAME} STATIC moci/moci.hpp moci/moci.cpp)
add_library(Moci::Moci ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Moci::Core
        Moci::Math
        Moci::Network
        Moci::System
        Moci::Graphics
        Moci::Application
        Moci::Widget
)

