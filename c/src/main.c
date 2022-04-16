#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "ui/console.h"

void (*fn_ptr)();

int main(void) {
    size_t dim = 10;

    uint8_t* cells = calloc(dim * dim, sizeof(uint8_t));

    game_t* game = game_create(cells, 10, 10);

    printf("Game Width = %ld, Height = %ld.\n", game->width, game->height);

    console_t* c = console_create(game);

    vis_t* console = (vis_t*)c;
    
    vis_init(console);

    vis_update(console);

    free_game(game);

    free_console(console);

    return 0;
}
