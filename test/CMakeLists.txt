
# Catch2 test framework
add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE external/catch2)

# Test executable
add_executable(libtaskman_test
    ../include/taskman/taskman.hpp
    src/test_sample_generator.hpp
    src/main.cpp
    src/taskman.test.cpp
    src/policy.test.cpp
)
    
target_link_libraries(libtaskman_test catch2 TaskMan::libtaskman)

add_test(NAME libtaskman_test COMMAND libtaskman_test)
