#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define     ALIVE       1
#define     DEAD        0

typedef struct {
    uint8_t* cells;
    size_t height;
    size_t width;
} game_t;

typedef struct {
    uint32_t x;
    uint32_t y;
} cell_t;

game_t* game_create(size_t height, size_t width);
void    free_game(game_t* game);

uint8_t is_alive(const game_t* game, const cell_t cell);

void    next_generation(game_t* game);

#endif