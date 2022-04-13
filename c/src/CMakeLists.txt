set(LIB_SOURCES
  game/game.c
)

set(MAIN_SOURCES
  main.c
)

set(HEADERS 
  game/game.h
)

add_library(game STATIC ${LIB_SOURCES} ${HEADERS})

add_executable(main ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(main game)

install(TARGETS game
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/game.h DESTINATION include)
