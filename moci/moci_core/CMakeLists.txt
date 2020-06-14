project(moc_core)

set(moci_core_sources
    moci_core.hpp
    moci_core.cpp
    
    benchmark/benchmark.hpp
    benchmark/profile.hpp

    core/algorithm.hpp
    core/logging.hpp
    core/logging.cpp
    core/memory.hpp
    core/preprocessor.hpp
    core/property_map.hpp
    core/property_map.cpp
    core/scope_guard.hpp
    core/scope_guard.cpp
    core/strings.hpp
    core/vector.hpp
)

add_library(moci_core_lib STATIC ${moci_core_sources})
add_library(Moci::Core ALIAS moci_core_lib)
target_compile_definitions(moci_core_lib PUBLIC NDEBUG=1)
target_link_libraries(moci_core_lib 
    PUBLIC 
        Moci::IncludePath
        Moci::CompilerOptions
        Moci::CompilerWarnings
        spdlog::spdlog
        GSL
    PRIVATE
        Moci::CodeCoverage
)


set (moci_core_test_src
    main_test.cpp
    
    core/algorithm_test.cpp
    core/property_map_test.cpp
    core/scope_guard_test.cpp
    core/strings_test.cpp
)
add_executable(moci_core_tests ${moci_core_test_src})
target_link_libraries(moci_core_tests 
    PRIVATE 
        Moci::Core 
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_core_test_src})
catch_discover_tests(moci_core_tests)


