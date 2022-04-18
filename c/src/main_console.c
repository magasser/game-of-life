#include <stdlib.h>
#include <stdio.h>

#include "helpers.h"
#include "game/game.h"
#include "ui/console/console.h"
#include "runner.h"
#include "io/text_reader.h"

#define ARGC    2

uint32_t main(uint32_t argc, char* argv[]) {
    reader_t* reader = (reader_t*)txt_reader_create();

    if (argc != ARGC) {
        printf_err("Expected %d arguments but received %ld.",
                ARGC, argc);
        exit(EXIT_FAILURE);
    }

    game_t* game = file_import(reader, argv[1]);

    console_t* console = console_create(game);

    vis_t* vis = (vis_t*)console;

    runner_t* r = runner_create(game, vis);

    runner_start(r);

    free_runner(r);
    free_reader(reader);
    free_console(console);
    free_game(game);

    return 0;
}
