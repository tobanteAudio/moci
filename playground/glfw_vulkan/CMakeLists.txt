project(glfw_vulkan)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES})