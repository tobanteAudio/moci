add_library(compiler_options INTERFACE)
add_library(Moci::CompilerOptions ALIAS compiler_options)

# Language standard
target_compile_features(compiler_options INTERFACE cxx_std_17)

# Sanitizers
set(SANITIZERS "")
# Address
if(MOCI_BUILD_ASAN AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  list(APPEND SANITIZERS "address")
endif()

# Undefined
if(MOCI_BUILD_UBSAN AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  list(APPEND SANITIZERS "undefined")
endif()

# Memory
if(MOCI_BUILD_MSAN AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  list(APPEND SANITIZERS "memory")
endif()

# Thread
if(MOCI_BUILD_TSAN AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  list(APPEND SANITIZERS "thread")
endif()


list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)
if(LIST_OF_SANITIZERS)
  if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
    target_compile_options(compiler_options INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
    target_link_libraries(compiler_options INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
  endif()
endif()