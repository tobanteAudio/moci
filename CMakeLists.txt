cmake_minimum_required(VERSION 3.23)
project(moci-dev LANGUAGES CXX)

option(MOCI_BUILD_COVERAGE       "Build with coverage enabled"                  OFF)
option(MOCI_BUILD_TOOLS          "Build tools"                                  OFF)
option(MOCI_BUILD_PLAYGROUND     "Build playground examples"                    OFF)
option(MOCI_API_OPENGL_LEGACY    "Use OpenGL Legacy implementation"             OFF)
option(MOCI_API_OPENGL_MODERN    "Use OpenGL 4.0 implementation"                ON)
option(MOCI_LINK_OPENGL_ES       "Link against OpenGL ES 2.0"                   OFF)

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(MociCodeCoverage)
include(MociCompilerOptions)
include(MociCompilerWarnings)

if(EMSCRIPTEN)
  add_subdirectory(src/app/playground/sdl_emscripten)
else()
  include(MociConan)

  enable_testing()
  include(CTest)
  find_package(Catch2 REQUIRED)
  include(Catch)

  add_subdirectory("src")
endif()
