# Code Coverage Configuration
add_library(coverage_config INTERFACE)
add_library(Moci::CodeCoverage ALIAS coverage_config)

if(Moci_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
  target_link_libraries(coverage_config INTERFACE --coverage)
  target_link_options(coverage_config INTERFACE --coverage)
endif()
