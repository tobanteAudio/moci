project(glfw_vulkan)

find_package(Vulkan REQUIRED)

# add_custom_command(
#     TARGET ${PROJECT_NAME}
#     COMMAND "glslangValidator -V "
#     APPEND COMMAND shader/fragment.frag
#     MAIN_DEPENDENCY shader/fragment.frag
#     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
# )


set(KERNELS
${PROJECT_SOURCE_DIR}/shader/vertex.vert
${PROJECT_SOURCE_DIR}/shader/fragment.frag
)

set(COMPILED_KERNELS
${PROJECT_SOURCE_DIR}/shader/vertex.vert.spv
${PROJECT_SOURCE_DIR}/shader/fragment.frag.spv
)

add_custom_target(shaders ALL DEPENDS ${KERNELS} ${COMPILED_KERNELS})

foreach(KERNEL ${KERNELS})
    add_custom_command(
        OUTPUT ${KERNEL}.spv
        COMMAND glslangValidator -V ${KERNEL} -g -o ${KERNEL}.spv
        DEPENDS ${KERNEL}
        COMMENT "Rebuilding ${KERNEL}.spv" )
        message(STATUS "Generating build commands for ${KERNEL}.spv"
    )
endforeach()


add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES})