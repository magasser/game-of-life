#ifndef GAME_HEADER
#define GAME_HEADER

#include <stdint.h>

typedef struct {
    uint8_t* cells;
    size_t height;
    size_t width;
} game_t;

game_t* create_game(size_t height, size_t width);
void    free_game(game_t* game_ptr);

#endif