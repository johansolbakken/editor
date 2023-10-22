cmake_minimum_required(VERSION 3.15)

project(editor)

set(CMAKE_C_STANDARD 11)

add_subdirectory(vendor/raylib)

add_executable(editor src/main.c)
target_link_libraries(editor raylib)
target_include_directories(editor PUBLIC vendor/raylib/src)
target_compile_options(editor PUBLIC -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code)