cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)

project(GameOfLife C)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})