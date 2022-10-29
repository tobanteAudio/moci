project(moci_system)

# SYSTEM
set(moci_system_sources
    system/info.hpp
    system/info.cpp
    system/apple/info.hpp
    system/apple/info.cpp
    system/linux/info.hpp
    system/linux/info.cpp
    system/windows/info.hpp
    system/windows/info.cpp
)

add_library(moci_system_lib STATIC ${moci_system_sources})
add_library(Moci::System ALIAS moci_system_lib)
set_target_properties(moci_system_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_system_lib
    PUBLIC
        Moci::Core
        Moci::Threads
    PRIVATE
        Moci::CodeCoverage
)


set (moci_system_test_src
    main_test.cpp

    system/info_test.cpp
)
add_executable(moci_system_tests ${moci_system_test_src})
set_target_properties(moci_system_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_system_tests
    PRIVATE
        Moci::System
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_system_test_src})
catch_discover_tests(moci_system_tests)
