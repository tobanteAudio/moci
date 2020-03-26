cmake_minimum_required (VERSION 3.13)
project(MociLibrary LANGUAGES CXX)


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE her
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    if(APPLE)
    else()
      set(BUILD_SHARED_LIBS OFF)
    endif()


    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # LTO
    include(CheckIPOSupported)
    check_ipo_supported(RESULT MOCI_SUPPORTS_LTO)
    if(MOCI_SUPPORTS_LTO)
      message(STATUS "Moci: lto is supported, but NOT enabled.")
      # set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
      message(STATUS "Moci: lto is NOT supported.")
    endif()
endif()

option(MOCI_BUILD_TESTS          "Build the unit tests with Catch2"  ON)
option(MOCI_BUILD_COVERAGE       "Build with coverage enabled"       OFF)
option(MOCI_CLANG_TIDY           "Run clang-tidy with the compiler." OFF)
option(MOCI_CLANG_TIDY_FIX       "Run & auto fix clang-tidy with the compiler." OFF)
option(MOCI_GIT_SUBMODULE        "Check submodules during build"     ON)
option(MOCI_API_OPENGL_MODERN    "Use OpenGL 4.0 implementation"     OFF)
option(MOCI_API_OPENGL_LEGACY    "Use OpenGL Legacy implementation"  OFF)
option(MOCI_API_OPENGL_ES        "Link against OpenGL ES 2.0"        OFF)

# Cmake setup
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(MociCodeCoverage)
include(MociCompilerOptions)
include(MociCompilerWarnings)
include(MociGitSubmodules)
include(MociThreading)

# tidy, has to be enabled for each target.
if(MOCI_CLANG_TIDY)
  if(CMake_SOURCE_DIR STREQUAL CMake_BINARY_DIR)
    message(FATAL_ERROR "MOCI_CLANG_TIDY requires an out-of-source build!")
  endif()
  find_program(CLANG_TIDY_EXE NAMES clang-tidy)
  if(NOT CLANG_TIDY_EXE)
    message(WARNING "MOCI_CLANG_TIDY is ON but clang-tidy is not found!")
  endif()
  # Fix should only be run with 1 core. Otherwise it messes up the headers.
  if(MOCI_CLANG_TIDY_FIX)
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "--config=file -fix")
  else()
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif(MOCI_CLANG_TIDY_FIX)

  # Create a preprocessor definition that depends on .clang-tidy content so
  # the compile command will change when .clang-tidy changes.  This ensures
  # that a subsequent build re-runs clang-tidy on all sources even if they
  # do not otherwise need to be recompiled.  Nothing actually uses this
  # definition.  We add it to targets on which we run clang-tidy just to
  # get the build dependency on the .clang-tidy file.
  file(SHA1 ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy clang_tidy_sha1)
  set(CLANG_TIDY_DEFINITIONS "CLANG_TIDY_SHA1=${clang_tidy_sha1}")
  unset(clang_tidy_sha1)
endif(MOCI_CLANG_TIDY)
configure_file(.clang-tidy .clang-tidy COPYONLY)

# 3rd party libraries
# assimp
if(APPLE)
else()
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library")
endif()

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSBIN_EXPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSXML_EXPORTER OFF CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/assimp     EXCLUDE_FROM_ALL)

# other
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/freetype   EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glm        EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw       EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GSL        EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/spdlog     EXCLUDE_FROM_ALL)

# Only include glew on desktop
if (${MOCI_API_OPENGL_ES})
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glew   EXCLUDE_FROM_ALL)
endif()

# Testing
if(MOCI_BUILD_TESTS)
  list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
  enable_testing()
  add_subdirectory(3rd_party/Catch2 EXCLUDE_FROM_ALL)
  include(CTest)
  include(Catch)
endif()

# Custom cmake targets for external code
add_subdirectory(3rd_party/stb-cmake)
add_subdirectory(3rd_party/imgui-cmake)

# Project
add_subdirectory(moci)

add_subdirectory(sandbox)
add_subdirectory(sandbox3D)

add_subdirectory(tools/model-viewer)

add_subdirectory(playground)