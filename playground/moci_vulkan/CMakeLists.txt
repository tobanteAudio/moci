project(moci_vulkan)

if(MOCI_API_VULKAN)
    find_package(Vulkan REQUIRED)

    set(KERNELS
        ${PROJECT_SOURCE_DIR}/shader/vertex.vert
        ${PROJECT_SOURCE_DIR}/shader/fragment.frag
    )

    set(COMPILED_KERNELS
        ${PROJECT_SOURCE_DIR}/shader/vertex.vert.spv
        ${PROJECT_SOURCE_DIR}/shader/fragment.frag.spv
    )


    foreach(KERNEL ${KERNELS})
    add_custom_command(
        OUTPUT ${KERNEL}.spv
        COMMAND glslangValidator -V ${KERNEL} -g -o ${KERNEL}.spv
        DEPENDS ${KERNEL}
        COMMENT "Rebuilding ${KERNEL}.spv" )
        message(STATUS "Generating build commands for ${KERNEL}.spv"
        )
        endforeach()
        
        
    add_custom_target(${PROJECT_NAME}_Shaders ALL DEPENDS ${KERNELS} ${COMPILED_KERNELS})
    add_executable(${PROJECT_NAME} 
        src/main.cpp

        src/vk/vk.hpp
        src/vk/vk.cpp

        src/application.hpp
        src/application.cpp
        src/window.hpp
        src/window.cpp
    )
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/glfw/include ${Vulkan_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES} glm::glm GSL)
endif()
