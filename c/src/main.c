#include <stdio.h>

#include "game/game.h"

int main(void) {
    game_t* game = game_create(10, 10);

    printf("Game Width = %ld, Height = %ld.\n", game->width, game->height);

    return 0;
}