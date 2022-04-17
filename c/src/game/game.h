#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define     ALIVE       1
#define     DEAD        0

typedef struct {
    uint64_t gen;
    uint64_t alive;
    uint64_t died;
    uint64_t revived;
} gen_stat_t;

typedef struct {
    uint8_t* cells;
    size_t width;
    size_t height;
    gen_stat_t gen;
} game_t;

typedef struct {
    int32_t x;
    int32_t y;
} cell_t;

game_t* game_create(uint8_t* cells, size_t width, size_t height);
void    free_game(game_t* game);

void    game_init(game_t* game);

uint8_t is_alive(const game_t* game, const cell_t cell);

void    next_gen(game_t* game);

char*   to_string(const game_t* game);

#endif