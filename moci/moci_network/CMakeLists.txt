project(moci_network)

set(moci_network_sources
    moci_network.hpp
    moci_network.cpp

    datagram/datagram.hpp
    datagram/datagram.cpp

    datagram/unix/datagram.hpp
    datagram/unix/datagram.cpp

    datagram/windows/datagram.hpp
    datagram/windows/datagram.cpp
)

add_library(moci_network_lib STATIC ${moci_network_sources})
add_library(Moci::Network ALIAS moci_network_lib)
target_link_libraries(moci_network_lib 
    PUBLIC 
        Moci::Core 
        Moci::Threads
    PRIVATE
        Moci::CodeCoverage
)

set (moci_network_test_src main_test.cpp datagram/datagram_test.cpp)
add_executable(moci_network_tests ${moci_network_test_src})
target_link_libraries(moci_network_tests 
    PRIVATE 
        Moci::Network 
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_network_test_src})
catch_discover_tests(moci_network_tests)
