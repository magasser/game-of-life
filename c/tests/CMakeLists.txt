include(FetchCMocka)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(TEST_GAME_SOURCES
  game/test_game.c
)

add_executable(test_game ${TEST_GAME_SOURCES})

target_link_libraries(test_game PRIVATE game cmocka-static)
