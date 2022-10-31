project(moci_events)

add_library(moci_events_lib)
add_library(Moci::Events ALIAS moci_events_lib)

target_sources(moci_events_lib
    PRIVATE
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


target_link_libraries(moci_events_lib
    PUBLIC
        Moci::Core
        Moci::CodeCoverage
)


add_executable(moci_events_tests main.test.cpp)
catch_discover_tests(moci_events_tests)

target_sources(moci_events_tests
    PRIVATE
        layer/layer_stack.test.cpp

        types/application_event.test.cpp
        types/key_event.test.cpp
        types/mouse_event.test.cpp
)

target_link_libraries(moci_events_tests
    PRIVATE
        Moci::Events
        Moci::CodeCoverage
        Catch2::Catch2
)
