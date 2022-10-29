cmake_minimum_required(VERSION 3.21)
project(MociLibrary LANGUAGES CXX)

option(MOCI_BUILD_NATIVE         "Build optimized for native platform"          OFF)
option(MOCI_BUILD_LTO            "Build with link time optimization"            OFF)
option(MOCI_BUILD_FAST_MATH      "Build with fast math optimizations"           OFF)
option(MOCI_BUILD_FRAMEPOINTER   "Build with frame pointer. For profiles"       OFF)
option(MOCI_BUILD_TESTS          "Build the unit tests with Catch2"             ON)
option(MOCI_BUILD_COVERAGE       "Build with coverage enabled"                  OFF)
option(MOCI_BUILD_SANDBOX        "Build sandbox applications"                   ON)
option(MOCI_BUILD_TOOLS          "Build tools"                                  OFF)
option(MOCI_BUILD_PLAYGROUND     "Build playground examples"                    OFF)
option(MOCI_API_VULKAN           "Use Vulkan API"                               ON)
option(MOCI_API_OPENGL_LEGACY    "Use OpenGL Legacy implementation"             ON)
option(MOCI_API_OPENGL_MODERN    "Use OpenGL 4.0 implementation"                OFF)
option(MOCI_LINK_OPENGL_ES       "Link against OpenGL ES 2.0"                   OFF)

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

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(MociCodeCoverage)
include(MociCompilerOptions)
include(MociCompilerWarnings)
include(MociConan)
include(MociThreading)

# other
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

# Only include glew on desktop
if (NOT ${MOCI_LINK_OPENGL_ES})
    find_package(GLEW REQUIRED)
endif()

# Testing
if(MOCI_BUILD_TESTS)
  enable_testing()
  include(CTest)
  find_package(Catch2 REQUIRED)
  include(Catch)
endif(MOCI_BUILD_TESTS)

add_subdirectory(moci)

if(MOCI_BUILD_SANDBOX)
  add_subdirectory(sandbox2D)
  add_subdirectory(sandbox3D)
  add_subdirectory(sandboxOpenGL)
endif(MOCI_BUILD_SANDBOX)

if(MOCI_BUILD_TOOLS)
  add_subdirectory(tools/model-viewer)
endif(MOCI_BUILD_TOOLS)

if(MOCI_BUILD_PLAYGROUND)
  add_subdirectory(playground)
endif(MOCI_BUILD_PLAYGROUND)
