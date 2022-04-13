cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)

set(CMAKE_C_COMPILER "C:/Strawberry/c/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Strawberry/c/bin/g++.exe")

project(GameOfLife C)

set(CHECK_INSTALL_DIR "C:/Program Files (x86)/Check")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Check REQUIRED)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
add_test(NAME check_game COMMAND check_game)
