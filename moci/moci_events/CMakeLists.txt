project(moci_events)

set(moci_events_sources
    events/types/application_event.hpp
    events/types/event.hpp
    events/types/key_event.hpp
    events/types/mouse_event.hpp
    events/input.hpp
    events/input.cpp
    events/key_codes.hpp
    events/mouse_button_codes.hpp
    events/timestep.hpp
)

add_library(moci_events_lib STATIC ${moci_events_sources})
add_library(Moci::Events ALIAS moci_events_lib)
set_target_properties(moci_events_lib PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_events_lib
    PUBLIC
        Moci::Core
    PRIVATE
        Moci::CodeCoverage
)


set (moci_events_test_src
    main_test.cpp

    events/types/application_event_test.cpp
    events/types/key_event_test.cpp
    events/types/mouse_event_test.cpp
)
add_executable(moci_events_tests ${moci_events_test_src})
set_target_properties(moci_events_tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(moci_events_tests
    PRIVATE
        Moci::Events
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_events_test_src})
catch_discover_tests(moci_events_tests)
