#include <stdlib.h>
#include <stdio.h>

#include "helpers.h"
#include "game/game.h"
#include "ui/gl/gl.h"
#include "io/text_reader.h"
#include "runner.h"

#define ARGC    2

uint32_t main(uint32_t argc, char* argv[]) {
    reader_t* reader = (reader_t*)txt_reader_create();

    if (argc != ARGC) {
        printf_err("Expected %d arguments but received %ld.",
                ARGC, argc);
        exit(EXIT_FAILURE);
    }

    game_t* game = file_import(reader, argv[1]);

    gl_t* gl = gl_create(game, 1920, 1080);

    vis_t* vis = (vis_t*)gl;

    runner_t* r = runner_create(game, vis);

    runner_start(r);

    free(r);
    free_gl(gl);
    free_reader(reader);
    free_game(game);

    return 0;
}
