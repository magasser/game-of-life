#include <stdlib.h>
#include <stdio.h>

#include "helpers.h"
#include "game/game.h"
#include "ui/console.h"
#include "runner.h"
#include "io/text_reader.h"

#define ARGC    2

uint32_t main(uint32_t argc, char* argv[]) {
    reader_t* reader = (reader_t*)txt_reader_create();

    if (argc != ARGC) {
        printf_err("Expected %d arguments but received %ld.",
                ARGC, argc);
        exit(errno);
    }

    game_t* game = file_import(reader, argv[1]);

    console_t* console = console_create(game);

    vis_t* vis = (vis_t*)console;

    runner_t* r = runner_create(game, vis);

    runner_start(r);

    free_game(game);

    free_console(console);

    free_reader(reader);

    return 0;
}
