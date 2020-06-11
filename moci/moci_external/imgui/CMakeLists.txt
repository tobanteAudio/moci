project(moci-imgui)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imgui.h
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imstb_truetype.h
    ${CMAKE_SOURCE_DIR}/3rd_party/imgui/imgui_demo.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(Moci::ImGui ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/imgui)
