#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "game.h"

#define     NR_OF_NEIGHBOURS        8
#define     NEIGHBOUR_RADIUS        1

/* Function delcarations */
void        apply_rules(const game_t* game, game_t* out_game, cell_t cell);
uint8_t*    get_neighbours(const game_t* game, cell_t cell);
uint8_t     nr_alive_neighbours(const uint8_t* neighbours);
uint8_t     get_cell(const game_t* game, cell_t cell);
void        set_cell(game_t* game, cell_t cell, uint8_t value);
void        copy_cells(const uint8_t* from, uint8_t* to, uint64_t length);
void        update_game(const game_t* new_game, game_t* game);
game_t*     copy_game(const game_t* game);
void        ensure_cell_in_bounds(const game_t* game, cell_t cell);
void        ensure_games_same_size(const game_t* g1, const game_t* g2);
/* End Function delcarations */

game_t* game_create(uint8_t* cells, size_t width, size_t height) {
    if (cells == NULL) {
        fprintf(stderr, "Cells cannot be null pointer.\n");
        exit(EXIT_FAILURE);
    }

    if (width == 0 || height == 0) {
        fprintf(stderr, "Dimensions of game must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    if (width > INT32_MAX || height > INT32_MAX) {
        fprintf(stderr, "Dimensions of game cannot be larger than %ld.\n", INT32_MAX);
        exit(EXIT_FAILURE);
    }

    game_t* game = malloc(sizeof(game_t));

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
    ensure_cell_in_bounds(game, cell);

    return get_cell(game, cell) == ALIVE;
}

void next_generation(game_t* game) {
    cell_t cell;

    game_t* next_gen = copy_game(game);

    uint64_t length = game->height * game->width;
    for (uint64_t i = 0; i < length; ++i) {
        cell.x = i % game->width;
        cell.y = i / game->width;

        apply_rules(game, next_gen, cell);
    }

    update_game(next_gen, game);

    free_game(next_gen);
}

void copy_cells(const uint8_t* from, uint8_t* to, uint64_t length) {
    for (uint64_t i = 0; i < length; ++i) {
        to[i] = from[i];
    }
}

game_t* copy_game(const game_t* game) {
    uint64_t length = game->width * game->height;
    uint8_t* cells = calloc(game->width * game->height, sizeof(uint8_t));
    
    copy_cells(game->cells, cells, length);

    game_t* copy = game_create(cells, game->width, game->height);

    return copy;
}

void update_game(const game_t* new_game, game_t* game) {
    ensure_games_same_size(new_game, game);

    uint64_t length = new_game->width * new_game->height;
    
    copy_cells(new_game->cells, game->cells, length);
}

void set_cell(game_t* game, cell_t cell, uint8_t value) {
    ensure_cell_in_bounds(game, cell);

    game->cells[cell.y * game->width + cell.x] = value;
}

uint8_t get_cell(const game_t* game, cell_t cell) {
    ensure_cell_in_bounds(game, cell);

    return game->cells[cell.y * game->width + cell.x];
}

void apply_rules(const game_t* game, game_t* out_game, cell_t cell) {
    uint8_t alive = is_alive(game, cell);
    uint8_t* neighbours = get_neighbours(game, cell);
    uint8_t alive_neighbours = nr_alive_neighbours(neighbours);

    if (alive) {
        /* Any live cell with less than two or more than three live neighbours dies */
        if (alive_neighbours < 2 || alive_neighbours > 3) {
            set_cell(out_game, cell, DEAD);
        }
    } else {
        /* Any dead cell with three live neighbours becomes a live cell */
        if (alive_neighbours == 3) {
            set_cell(out_game, cell, ALIVE);
        }
    }

    free(neighbours);
}

uint8_t* get_neighbours(const game_t* game, cell_t cell) {
    uint8_t* neighbours = calloc(NR_OF_NEIGHBOURS, sizeof(uint8_t));

    uint8_t i = 0;
    int32_t x_max_value = fmin(cell.x + NEIGHBOUR_RADIUS, game->width - 1);
    int32_t y_max_value = fmin(cell.y + NEIGHBOUR_RADIUS, game->height - 1);
    for (uint64_t y = fmax(0, cell.y - NEIGHBOUR_RADIUS); y <= y_max_value; ++y) {
        for (uint64_t x = fmax(0, cell.x - NEIGHBOUR_RADIUS) && i < NR_OF_NEIGHBOURS; x <= x_max_value; ++x, ++i) {
            if (x != cell.x || y != cell.y) {
                neighbours[i] = game->cells[y * game->width + x];
            }
        }
    }

    return neighbours;
}

uint8_t nr_alive_neighbours(const uint8_t* neighbours) {
    uint8_t count = 0;

    for (uint8_t i = 0; i < NR_OF_NEIGHBOURS; ++i) {
        if (neighbours[i]) {
            ++count;
        }
    }

    return count;
}

void ensure_cell_in_bounds(const game_t* game, cell_t cell) {    
    if (cell.x >= game->width || cell.y >= game->height) {
        fprintf(stderr, 
                "Tried to access cell out of bounds. Width=%zu / Height=%zu, X=%llu / Y=%llu.\n", 
                game->width, game->height, cell.x, cell.y);
        
        exit(EXIT_FAILURE);
    }
}

void ensure_games_same_size(const game_t* g1, const game_t* g2) {
    if (g1->width != g2->width || g1->height != g2->height) {
        fprintf(stderr,
                "Games not same size. G1: %lldx%lld, G2: %lldx%lld",
                g1->width, g1->height, g2->width, g2->height);
        
        exit(EXIT_FAILURE);
    }
}
