project(moci_events)

set(moci_events_sources
    events/application_event.hpp
    events/event.hpp
    events/key_event.hpp
    events/mouse_event.hpp
    input.hpp
    input.cpp
    key_codes.hpp
    mouse_button_codes.hpp
    timestep.hpp
)

add_library(moci_events_lib STATIC ${moci_events_sources})
add_library(Moci::Events ALIAS moci_events_lib)
target_link_libraries(moci_events_lib PUBLIC Moci::Core)


set (moci_events_test_src
    main_test.cpp
    
    events/application_event_test.cpp
    events/key_event_test.cpp
    events/mouse_event_test.cpp
)
add_executable(moci_events_tests ${moci_events_test_src})
target_link_libraries(moci_events_tests PRIVATE Moci::Events Catch2::Catch2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_events_test_src})
catch_discover_tests(moci_events_tests)
