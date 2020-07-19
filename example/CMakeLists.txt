
# Example executable
add_executable(libtaskman_example
    ../include/taskman/taskman.hpp
    ../test/src/test_sample_generator.hpp
    src/text_ui.hpp
    src/taskman_example.cpp
)
    
target_link_libraries(libtaskman_example TaskMan::libtaskman)
