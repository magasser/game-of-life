#include <stdlib.h>
#include <stdio.h>

#include "game.h"

void init_cells(uint8_t* cells, size_t length);

game_t* create_game(size_t width, size_t height) {
    game_t* game = malloc(sizeof(game_t));
    uint8_t* cells = malloc(width * height * sizeof(uint8_t));

    init_cells(cells, width * height);

    game->cells = cells;
    game->height = height;
    game->width = width;

    return game;
}

void init_cells(uint8_t* cells, size_t length) {
    for (uint64_t aa = 0; aa < length; ++aa) {
        cells[aa] = 0;
    }
}

void free_game(game_t* game) {
    free(&game->cells);
}

uint8_t is_cell_alive(const game_t* game, uint64_t x, uint64_t y) {
    if (x >= game->width || y >= game->height) {
        fprintf(stderr, "Read cell out of bounds. Width=%zu / Height=%zu, X=%llu / Y=%llu.\n", game->width, game->height, x, y);
        exit(EXIT_FAILURE);
    }

    return game->cells[y * game->width + x];
}