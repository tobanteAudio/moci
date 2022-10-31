project(moci_core)

find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

add_library(moci_core)
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

add_executable(moci_core_tests main.test.cpp)
catch_discover_tests(moci_core_tests)

target_sources(moci_core_tests
    PRIVATE
        property_map.test.cpp
        scope_guard.test.cpp
        strings.test.cpp

        geometry/line.test.cpp
        geometry/rectangle.test.cpp
        geometry/point.test.cpp

        network/datagram.test.cpp

        system/info.test.cpp
)

target_link_libraries(moci_core_tests
    PRIVATE
        Moci::Core
        Moci::CodeCoverage
        Catch2::Catch2
)

