cmake_minimum_required(VERSION 3.13...3.16)
project(MociLibrary LANGUAGES CXX)

# LTO
cmake_policy(SET CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# OpenGL
cmake_policy(SET CMP0072 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)

option(MOCI_GIT_SUBMODULE        "Check submodules during build"                ON)
option(MOCI_BUILD_NATIVE         "Build optimized for native platform"          ON)
option(MOCI_BUILD_LTO            "Build with link time optimization"            ON)
option(MOCI_BUILD_FAST_MATH      "Build with fast math optimizations"           OFF)
option(MOCI_BUILD_FRAMEPOINTER   "Build with frame pointer. For profiles"       OFF)
option(MOCI_BUILD_TESTS          "Build the unit tests with Catch2"             ON)
option(MOCI_BUILD_COVERAGE       "Build with coverage enabled"                  OFF)
option(MOCI_BUILD_SANDBOX        "Build sandbox applications"                   ON)
option(MOCI_BUILD_TOOLS          "Build tools"                                  OFF)
option(MOCI_BUILD_PLAYGROUND     "Build playground examples"                    OFF)
option(MOCI_API_OPENGL_LEGACY    "Use OpenGL Legacy implementation"             ON)
option(MOCI_API_OPENGL_MODERN    "Use OpenGL 4.0 implementation"                OFF)
option(MOCI_API_OPENGL_ES        "Link against OpenGL ES 2.0"                   OFF)
option(MOCI_API_VULKAN           "Link against Vulkan"                          ON)

# Only do these if this is the main project,
# and not if it is included through add_subdirectory.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    if(NOT APPLE)
      set(BUILD_SHARED_LIBS OFF)
    endif()

    add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

    if(MOCI_BUILD_NATIVE)
      include(CheckCXXCompilerFlag)
      CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_OPT_ARCH_NATIVE_SUPPORTED)
      if (COMPILER_OPT_ARCH_NATIVE_SUPPORTED)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
      endif()

      if(MSVC)
        CHECK_CXX_COMPILER_FLAG("/arch:AVX2" COMPILER_OPT_ARCH_AVX2_SUPPORTED)
        if(COMPILER_OPT_ARCH_AVX2_SUPPORTED)
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
        endif()
      endif()
    endif(MOCI_BUILD_NATIVE)
    
    if(MOCI_BUILD_FRAMEPOINTER)
      CHECK_CXX_COMPILER_FLAG("-fno-omit-frame-pointer" COMPILER_NO_OMIT_FRAMEPOINTER_SUPPORTED)
      if (COMPILER_NO_OMIT_FRAMEPOINTER_SUPPORTED)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
      endif()
    endif(MOCI_BUILD_FRAMEPOINTER)

    if(MOCI_BUILD_FAST_MATH)
      CHECK_CXX_COMPILER_FLAG("-ffast-math" COMPILER_OPT_FAST_MATH_SUPPORTED)
      if (COMPILER_OPT_FAST_MATH_SUPPORTED)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
      endif() 
    endif(MOCI_BUILD_FAST_MATH)

    if(MOCI_BUILD_LTO)
      include(CheckIPOSupported)
      check_ipo_supported(RESULT MOCI_SUPPORTS_LTO)
      if(MOCI_SUPPORTS_LTO)
        message(STATUS "Moci: lto is supported & enable")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
      else()
        message(STATUS "Moci: lto is NOT supported")
      endif()
  endif(MOCI_BUILD_LTO)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(MociCodeCoverage)
include(MociCompilerOptions)
include(MociCompilerWarnings)
include(MociGitSubmodules)
include(MociThreading)

# 3rd party libraries
# assimp
if(NOT APPLE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library")
endif()

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSBIN_EXPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_ASSXML_EXPORTER ON CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/assimp     EXCLUDE_FROM_ALL)

# other
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/freetype   EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glm        EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw       EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GSL        EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/spdlog     EXCLUDE_FROM_ALL)

# Only include glew on desktop
if (NOT ${MOCI_API_OPENGL_ES})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glew   EXCLUDE_FROM_ALL)
endif()

# Testing
if(MOCI_BUILD_TESTS)
  list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
  enable_testing()
  add_subdirectory(3rd_party/Catch2 EXCLUDE_FROM_ALL)
  include(CTest)
  include(Catch)
endif(MOCI_BUILD_TESTS)

add_subdirectory(moci)

if(MOCI_BUILD_SANDBOX)
  add_subdirectory(sandbox)
  add_subdirectory(sandbox3D)
endif(MOCI_BUILD_SANDBOX)

if(MOCI_BUILD_TOOLS)
  add_subdirectory(tools/model-viewer)
endif(MOCI_BUILD_TOOLS)

if(MOCI_BUILD_PLAYGROUND)
  add_subdirectory(playground)
endif(MOCI_BUILD_PLAYGROUND)
