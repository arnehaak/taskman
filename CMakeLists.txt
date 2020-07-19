cmake_minimum_required(VERSION 3.13...3.16 FATAL_ERROR)

project(taskman VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)


# Create library target
# (adding headers here only so that they show up in the IDE)
add_library(libtaskman
    include/taskman/taskman.hpp
    include/taskman/policy_iface.hpp
    include/taskman/policy_blocking.hpp
    include/taskman/policy_fifo.hpp
    include/taskman/policy_prio.hpp
    include/taskman/heap.hpp
    include/taskman/process.hpp
    include/taskman/taskman_iface.hpp
    include/taskman/taskman_common.hpp
    include/taskman/taskman_fifo.hpp
    include/taskman/taskman_prio.hpp
    src/policy_blocking.cpp
    src/policy_fifo.cpp
    src/policy_prio.cpp
    src/taskman_common.cpp
)



#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(TaskMan::libtaskman ALIAS libtaskman)

#Set target properties
target_include_directories(libtaskman
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(libtaskman PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)



# Test
enable_testing()
add_subdirectory(test)

# Usage example
add_subdirectory(example)
