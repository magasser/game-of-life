#include <stdio.h>

#include "game/game.h"

int main(void) {
    
    game_t* game = create_game(100, 100);

    printf("Height = %zu, Width = %zu\n", game->height, game->width);
    printf("Is cell X=%llu / Y=%llu alive: %hhu\n", 101, 50, is_cell_alive(game, 101, 50));

    free_game(game);
    return 0;
}