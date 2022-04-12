#include <stdio.h>

#include "game/game.h"

int main() {
    
    game_t* game = create_game(100, 100);

    printf("Height = %zu, Width = %zu", game->height, game->width);

    free_game(game);
    return 0;
}