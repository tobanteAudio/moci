cmake_minimum_required(VERSION 3.21)
project(moci-dev LANGUAGES CXX)

option(MOCI_BUILD_FRAMEPOINTER   "Build with frame pointer. For profiles"       OFF)
option(MOCI_BUILD_COVERAGE       "Build with coverage enabled"                  OFF)
option(MOCI_BUILD_TOOLS          "Build tools"                                  OFF)
option(MOCI_BUILD_PLAYGROUND     "Build playground examples"                    OFF)
option(MOCI_API_OPENGL_LEGACY    "Use OpenGL Legacy implementation"             ON)
option(MOCI_API_OPENGL_MODERN    "Use OpenGL 4.0 implementation"                OFF)
option(MOCI_LINK_OPENGL_ES       "Link against OpenGL ES 2.0"                   OFF)

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Let's nicely support folders in IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT APPLE)
  set(BUILD_SHARED_LIBS OFF)
endif()

add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

if(MOCI_BUILD_FRAMEPOINTER)
  check_cxx_compiler_flag("-fno-omit-frame-pointer" COMPILER_NO_OMIT_FRAMEPOINTER_SUPPORTED)
  if (COMPILER_NO_OMIT_FRAMEPOINTER_SUPPORTED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
  endif()
endif(MOCI_BUILD_FRAMEPOINTER)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(MociCodeCoverage)
include(MociCompilerOptions)
include(MociCompilerWarnings)

if(EMSCRIPTEN)
  add_subdirectory(playground/sdl_emscripten)
else()
  include(MociConan)

  if(WIN32)
    find_package(SDL2 REQUIRED)
  endif()

  enable_testing()
  include(CTest)
  find_package(Catch2 REQUIRED)
  include(Catch)

  add_subdirectory("src")
endif()


