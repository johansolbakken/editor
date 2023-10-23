cmake_minimum_required(VERSION 3.15)

project(editor)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor/raylib)

find_package(LLVM REQUIRED CONFIG)
set(CLANG_INCLUDE_DIRS "llvm/clang/include")
set(CLANG_LIBS clang)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(editor ${SOURCES})
target_link_libraries(editor raylib ${LLVM_LIBRARIES} ${LLVM_SYSTEM_LIBS} ${CLANG_LIBS})
target_include_directories(editor PUBLIC vendor/raylib/src ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
target_link_directories(editor PUBLIC ${LLVM_LIBRARY_DIRS})
target_compile_definitions(editor PUBLIC ${LLVM_DEFINITIONS})
target_compile_options(editor PUBLIC -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

install(TARGETS editor DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)