#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "game.h"

#define NR_OF_NEIGHBOURS 8

void        apply_rules(game_t* game, cell_t cell);
uint8_t*    get_neighbours(game_t* game, cell_t cell);
uint8_t     nr_alive_neighbours(uint8_t* neighbours);
uint8_t     get_cell(const game_t* game, cell_t cell);
void        set_cell(game_t* game, cell_t cell, uint8_t value);

game_t* game_create(size_t width, size_t height) {
    if (width == 0 || height == 0) {
        fprintf(stderr, "Dimensions of game must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    if (width > UINT32_MAX || height > UINT32_MAX) {
        fprintf(stderr, "Dimensions of game can not be bigger than %ld.\n", UINT32_MAX);
        exit(EXIT_FAILURE);
    }

    game_t* game = malloc(sizeof(game_t));
    uint8_t* cells = calloc(width * height, sizeof(uint8_t));

    game->cells = cells;
    game->height = height;
    game->width = width;

    return game;
}

void free_game(game_t* game) {
    free(game->cells);
    free(game);
}

uint8_t is_alive(const game_t* game, cell_t cell) {
    if (cell.x >= game->width || cell.y >= game->height) {
        fprintf(stderr, 
                "Read cell out of bounds. Width=%zu / Height=%zu, X=%llu / Y=%llu.\n", 
                game->width, game->height, cell.x, cell.y);
        
        exit(EXIT_FAILURE);
    }

    return get_cell(game, cell) == ALIVE;
}

void next_generation(game_t* game) {
    cell_t cell;

    uint64_t length = game->height * game->width;
    for (uint64_t i = 0; i < length; ++i) {
        cell.x = i % game->width;
        cell.y = i / game->width;

        apply_rules(game, cell);
    }
}

void set_cell(game_t* game, cell_t cell, uint8_t value) {
    game->cells[cell.y * game->width + cell.x] = value;
}

uint8_t get_cell(const game_t* game, cell_t cell) {
    return game->cells[cell.y * game->width + cell.x];
}

void apply_rules(game_t* game, cell_t cell) {
    uint8_t alive = is_alive(game, cell);
    uint8_t* neighbours = get_neighbours(game, cell);
    uint8_t alive_neighbours = nr_alive_neighbours(neighbours);

    if (alive) {
        /* Any live cell with less than two or more than three live neighbours dies */
        if (alive_neighbours < 2 || alive_neighbours > 3) {
            set_cell(game, cell, DEAD);
        }
    } else {
        /* Any dead cell with three live neighbours becomes a live cell */
        if (alive_neighbours == 3) {
            set_cell(game, cell, ALIVE);
        }
    }

    free(neighbours);
}

uint8_t* get_neighbours(game_t* game, cell_t cell) {
    uint8_t* neighbours = calloc(NR_OF_NEIGHBOURS, sizeof(uint8_t));

    uint8_t i = 0;
    for (uint64_t x = fmax(0, cell.x - 1); x <= fmin(cell.x + 1, game->width); ++x) {
        for (uint64_t y = fmax(0, cell.y -1); y <= fmin(cell.y + 1, game->height); ++y) {
            if (x != cell.x || y != cell.y) {
                neighbours[++i] = game->cells[y * game->width + x];
            }
        }
    }

    return neighbours;
}

uint8_t nr_alive_neighbours(uint8_t* neighbours) {
    uint8_t count;

    for (uint8_t i = 0; i < NR_OF_NEIGHBOURS; ++i) {
        if (neighbours[i]) {
            ++count;
        }
    }

    return count;
}
