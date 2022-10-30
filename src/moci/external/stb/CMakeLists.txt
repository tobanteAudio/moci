project(mc_stb)

find_package(stb REQUIRED)

add_library(stbimage STATIC src/stb_image.cpp)
add_library(Moci::StbImage ALIAS stbimage)
set_target_properties(stbimage PROPERTIES UNITY_BUILD OFF)
target_link_libraries(stbimage PUBLIC stb::stb)

add_library(stbimagewrite STATIC src/stb_image_write.cpp)
add_library(Moci::StbImageWrite ALIAS stbimagewrite)
set_target_properties(stbimagewrite PROPERTIES UNITY_BUILD OFF)
target_link_libraries(stbimagewrite PUBLIC stb::stb)

add_library(stbimageresize STATIC src/stb_image_resize.cpp)
add_library(Moci::StbImageResize ALIAS stbimageresize)
set_target_properties(stbimageresize PROPERTIES UNITY_BUILD OFF)
target_link_libraries(stbimageresize PUBLIC stb::stb)