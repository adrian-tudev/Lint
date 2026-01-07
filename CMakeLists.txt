cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "clang")

project(lint C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -O2 -g)

include_directories(src)

file(GLOB_RECURSE SRC_SOURCES "src/*.c")
file(GLOB_RECURSE TEST_SOURCES "tests/*.c")

set(LIB_SOURCES ${SRC_SOURCES})
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*/main.c$")

# Main executable target
add_executable(lint ${SRC_SOURCES})

find_library(READLINE_LIBRARY readline)
if(READLINE_LIBRARY)
    target_link_libraries(lint PRIVATE ${READLINE_LIBRARY})
    if(CURSES_LIBRARY)
        target_link_libraries(lint PRIVATE ${CURSES_LIBRARY})
    endif()
endif()

# Test executable target
add_executable(lint_test ${LIB_SOURCES} ${TEST_SOURCES})
target_include_directories(lint_test PRIVATE tests)
target_compile_definitions(lint_test PRIVATE TESTS)

# Custom target to run tests
add_custom_target(tests
    COMMAND lint_test
    DEPENDS lint_test
    COMMENT "Running tests"
)