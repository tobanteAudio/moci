project(moci_core)

find_package(spdlog REQUIRED)

add_library(moci_core STATIC ${moci_core_sources})
add_library(Moci::Core ALIAS moci_core)

target_sources(moci_core
    PRIVATE
        logging.hpp
        logging.cpp
        memory.hpp
        preprocessor.hpp
        property_map.hpp
        property_map.cpp
        scope_guard.hpp
        scope_guard.cpp
        span.hpp
        strings.hpp
        vector.hpp
)

set_target_properties(moci_core PROPERTIES UNITY_BUILD OFF)
target_compile_definitions(moci_core PUBLIC NDEBUG=1)
target_link_libraries(moci_core
    PUBLIC
        Moci::IncludePath
        Moci::CompilerOptions
        Moci::CompilerWarnings
        spdlog::spdlog
    PRIVATE
        Moci::CodeCoverage
)


set (moci_core_test_src
    main_test.cpp

    property_map_test.cpp
    scope_guard_test.cpp
    strings_test.cpp
)
add_executable(moci_core_tests ${moci_core_test_src})
set_target_properties(moci_core_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_core_tests
    PRIVATE
        Moci::Core
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_core_test_src})
catch_discover_tests(moci_core_tests)

