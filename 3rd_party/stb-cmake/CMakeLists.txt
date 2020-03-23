project(mc_stb)

add_library(stbimage STATIC src/stb_image.cpp)
add_library(Moci::StbImage ALIAS stbimage)
target_include_directories(stbimage PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/stb)

add_library(stbimagewrite STATIC src/stb_image_write.cpp)
add_library(Moci::StbImageWrite ALIAS stbimagewrite)
target_include_directories(stbimagewrite PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/stb)

add_library(stbimageresize STATIC src/stb_image_resize.cpp)
add_library(Moci::StbImageResize ALIAS stbimageresize)
target_include_directories(stbimageresize PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/stb)