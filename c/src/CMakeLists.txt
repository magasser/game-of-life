find_package(OpenGL REQUIRED)

include(FetchGLFW)
include(FetchGLEW)
include(FetchCGLM)

set(GAME_LIB_SOURCES
  game/game.c
  helpers.c
)

set(IO_LIB_SOURCES
  game/game.c
  io/file_reader.c
  io/text_reader.c
  io/rle_reader.c
  io/reader_factory.c
  helpers.c
)

set(RUNNER_LIB_SOURCES
  game/game.c
  ui/visualizer.c  
  runner.c
  helpers.c
)

set(CONSOLE_LIB_SOURCES
  game/game.c
  ui/visualizer.c
  ui/console/console.c
)

set(GL_LIB_SOURCES
  game/game.c
  ui/visualizer.c
  ui/gl/gl.c
  ui/gl/shaders/shaders.c
  helpers.c
)

set(MAIN_CONSOLE_SOURCES
  main_console.c
)

set(MAIN_GL_SOURCES
  main_gl.c
)

set(GAME_HEADERS
  game/game.h
  ui/visualizer.h
)

set(IO_HEADERS
  game/game.h
  io/file_reader.h
  io/text_reader.h
  io/rle_reader.h
  io/reader_factory.h
  helpers.h
)

set(RUNNER_HEADERS
  game/game.h
  ui/visualizer.h
  runner.h
  helpers.h
)

set(CONSOLE_HEADERS
  game/game.h
  ui/visualizer.h
  ui/console/console.h
  helpers.h
)

set(GL_HEADERS
  game/game.h
  ui/visualizer.h
  ui/gl/gl.h
  ui/gl/shaders/shaders.h
  helpers.h
)

add_library(game STATIC ${GAME_LIB_SOURCES} ${GAME_HEADERS})
add_library(io STATIC ${IO_LIB_SOURCES} ${IO_HEADERS})
add_library(runner STATIC ${RUNNER_LIB_SOURCES} ${RUNNER_HEADERS})
add_library(console STATIC ${CONSOLE_LIB_SOURCES} ${CONSOLE_HEADERS})
add_library(gl STATIC ${GL_LIB_SOURCES} ${GL_HEADERS})

set(GL_LIBS 
  OpenGL::GL
  glfw
  libglew_static
  cglm
)

add_executable(main_console ${GAME_HEADERS} ${IO_HEADERS} ${CONSOLE_HEADERS} ${MAIN_CONSOLE_SOURCES})
add_executable(main_gl ${GAME_HEADERS} ${IO_HEADERS} ${GL_HEADERS} ${MAIN_GL_SOURCES})

target_link_libraries(gl ${GL_LIBS})

target_link_libraries(main_console game io runner console)
target_link_libraries(main_gl game io runner gl)

# install(TARGETS game
#   RUNTIME DESTINATION bin
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib)

# install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/game.h DESTINATION include)
