#include <stdlib.h>

#include "game.h"

game_t* create_game(size_t width, size_t height) {
    game_t* game = malloc(sizeof(game_t));
    uint8_t* cells = malloc(width * height * sizeof(uint8_t));

    game->cells = cells;
    game->height = height;
    game->width = width;

    return game;
}

void free_game(game_t* game) {
    free(&game->cells);
}