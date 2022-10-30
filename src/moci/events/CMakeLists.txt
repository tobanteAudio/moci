project(moci_events)

set(moci_events_sources
    layer/layer_stack.hpp
    layer/layer_stack.cpp
    layer/layer.hpp
    layer/layer.cpp

    types/application_event.hpp
    types/event.hpp
    types/key_event.hpp
    types/mouse_event.hpp

    input.hpp
    input.cpp
    key_codes.hpp
    mouse_button_codes.hpp
    timestep.hpp
)

add_library(moci_events_lib STATIC ${moci_events_sources})
add_library(Moci::Events ALIAS moci_events_lib)
set_target_properties(moci_events_lib PROPERTIES UNITY_BUILD ON)
target_link_libraries(moci_events_lib
    PUBLIC
        Moci::Core
    PRIVATE
        Moci::CodeCoverage
)


set (moci_events_test_src
    main_test.cpp

    layer/layer_stack_test.cpp

    types/application_event_test.cpp
    types/key_event_test.cpp
    types/mouse_event_test.cpp
)
add_executable(moci_events_tests ${moci_events_test_src})
set_target_properties(moci_events_tests PROPERTIES UNITY_BUILD ON)
target_link_libraries(moci_events_tests
    PRIVATE
        Moci::Events
        Moci::CodeCoverage
        Catch2::Catch2
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${moci_events_test_src})
catch_discover_tests(moci_events_tests)
