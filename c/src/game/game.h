#ifndef GAME_H
#define GAME_H

#include <stdint.h>

typedef struct {
    uint8_t* cells;
    size_t height;
    size_t width;
} game_t;

game_t* create_game(size_t height, size_t width);
void    free_game(game_t* game);

uint8_t is_cell_alive(const game_t* game, uint64_t x, uint64_t y);

#endif