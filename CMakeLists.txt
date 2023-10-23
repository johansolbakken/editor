cmake_minimum_required(VERSION 3.15)

project(editor)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor/raylib)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(editor ${SOURCES})
target_link_libraries(editor raylib)
target_include_directories(editor PUBLIC vendor/raylib/src)
target_compile_options(editor PUBLIC -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if (APPLE) 
    set_target_properties(editor PROPERTIES MACOSX_BUNDLE TRUE)
endif()

install(TARGETS editor DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)