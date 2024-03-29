# Link this 'library' to use the standard warnings
add_library(compiler_warnings INTERFACE)
add_library(Moci::CompilerWarnings ALIAS compiler_warnings)

# Warnings
if(MSVC)
  target_compile_options(compiler_warnings INTERFACE /W4 "/permissive-" /wd4201)
else()
  target_compile_options(compiler_warnings INTERFACE -Wall -Wextra -Wpedantic -Wno-shadow)
endif()
