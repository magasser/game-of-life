include(FetchCMocka)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(TEST_SOURCES
  game/test_game.c
  test_main.c
)

set(TEST_HEADERS
  tests.h
)

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests PRIVATE game cmocka-static)
