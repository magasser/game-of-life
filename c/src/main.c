#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "ui/console.h"
#include "runner.h"

int main(void) {
    size_t dim = 3;

    uint8_t* cells = calloc(dim * dim, sizeof(uint8_t));

    cells[2] = ALIVE;
    cells[3] = ALIVE;
    cells[4] = ALIVE;
    cells[8] = ALIVE;

    game_t* game = game_create(cells, dim, dim);

    console_t* console = console_create(game);

    vis_t* vis = (vis_t*)console;

    runner_t* r = runner_create(game, vis);

    runner_start(r);

    free_game(game);

    free_console(console);

    return 0;
}
