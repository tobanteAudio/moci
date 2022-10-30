project(moci_system)

find_package(Threads REQUIRED)

set(moci_system_sources
    info.hpp
    info.cpp
    apple/info.hpp
    apple/info.cpp
    linux/info.hpp
    linux/info.cpp
    windows/info.hpp
    windows/info.cpp
)

add_library(moci_system_lib STATIC ${moci_system_sources})
add_library(Moci::System ALIAS moci_system_lib)
target_link_libraries(moci_system_lib
    PUBLIC
        Moci::Core
        Threads::Threads
    PRIVATE
        Moci::CodeCoverage
)


set (moci_system_test_src
    main_test.cpp

    info_test.cpp
)
add_executable(moci_system_tests ${moci_system_test_src})
target_link_libraries(moci_system_tests
    PRIVATE
        Moci::System
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_system_test_src})
catch_discover_tests(moci_system_tests)
