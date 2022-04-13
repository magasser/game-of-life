include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

set(TEST_SOURCES
  game/check_game.c
)

add_executable(check_game ${TEST_SOURCES})
target_link_libraries(check_game game ${CHECK_LIBRARIES})