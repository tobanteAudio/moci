project(moci_core)

find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

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

        geometry/line.hpp
        geometry/rectangle.hpp
        geometry/rectangle.cpp
        geometry/point.hpp

        network/datagram.hpp
        network/datagram.cpp
        network/unix/datagram.hpp
        network/unix/datagram.cpp
        network/windows/datagram.hpp
        network/windows/datagram.cpp

        system/info.hpp
        system/info.cpp
        system/apple/info.hpp
        system/apple/info.cpp
        system/linux/info.hpp
        system/linux/info.cpp
        system/windows/info.hpp
        system/windows/info.cpp
)

target_compile_definitions(moci_core PUBLIC NDEBUG=1)
target_link_libraries(moci_core
    PUBLIC
        Moci::IncludePath
        Moci::CompilerOptions
        Moci::CompilerWarnings

        spdlog::spdlog
        glm::glm
        Threads::Threads
    PRIVATE
        Moci::CodeCoverage
)


set (moci_core_test_src
    main_test.cpp

    property_map_test.cpp
    scope_guard_test.cpp
    strings_test.cpp

    geometry/line_test.cpp
    geometry/rectangle_test.cpp
    geometry/point_test.cpp

    network/datagram_test.cpp

    system/info_test.cpp
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

