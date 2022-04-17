cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_C_COMPILER "C:/Strawberry/c/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/Strawberry/c/bin/g++.exe")

set(ENV{UNIT_TESTING_DEBUG} 1)

set(EXECUTABLE_OUTPUT_PATH "./${CMAKE_BUILD_TYPE}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(GameOfLife C)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
add_test(NAME run_tests COMMAND run_tests)
