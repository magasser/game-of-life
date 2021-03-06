#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../helpers.h"
#include "game.h"

#define     NR_OF_NEIGHBOURS        8
#define     NEIGHBOUR_RADIUS        1

#define     NOTHING                 0
#define     DIED                    1
#define     REVIVED                 2

/* Function delcarations */
static uint8_t  apply_rules(const game_t* game, game_t* out_game, cell_t cell);
static uint8_t* get_neighbours(const game_t* game, cell_t cell);
static uint8_t  nr_alive_neighbours(const uint8_t* neighbours);
static uint64_t nr_alive_cells(const game_t* game);
static uint8_t  get_cell(const game_t* game, cell_t cell);
static void     set_cell(game_t* game, cell_t cell, uint8_t value);
static void     copy_cells(const uint8_t* from, uint8_t* to, uint64_t length);
static void     update_game(const game_t* new_game, game_t* game);
static game_t*  copy_game(const game_t* game);
static void     ensure_cell_in_bounds(const game_t* game, cell_t cell);
static void     ensure_games_same_size(const game_t* g1, const game_t* g2);
/* End Function delcarations */

game_t* game_create(uint8_t* cells, size_t width, size_t height) {
    if (cells == NULL) {
        printf_err("Cells cannot be null pointer.");
        exit(EXIT_FAILURE);
    }

    if (width == 0 || height == 0) {
        printf_err("Dimensions of game must be greater than 0.");
        exit(EXIT_FAILURE);
    }

    if (width > INT32_MAX || height > INT32_MAX) {
        printf_err("Dimensions of game cannot be larger than %ld.", INT32_MAX);
        exit(EXIT_FAILURE);
    }

    game_t* game = malloc(sizeof(game_t));

    game->cells = cells;
    game->height = height;
    game->width = width;

    game->gen.gen = 1;
    game->gen.alive = nr_alive_cells(game);
    game->gen.died = 0;
    game->gen.revived = 0;

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

void next_gen(game_t* game) {
    cell_t cell;

    game_t* next_gen = copy_game(game);

    game->gen.died = 0;
    game->gen.revived = 0;

    uint64_t length = game->height * game->width;
    for (uint64_t i = 0; i < length; ++i) {
        cell.x = i % game->width;
        cell.y = i / game->width;

        uint8_t state = apply_rules(game, next_gen, cell);

        if (state == DIED) {
            ++game->gen.died;
            --game->gen.alive;
        } else if (state == REVIVED) {
            ++game->gen.revived;
            ++game->gen.alive;
        }
    }

    update_game(next_gen, game);

    ++game->gen.gen;

    free_game(next_gen);
}

char* to_string(const game_t* game) {
    const uint8_t BUFFER_SIZE = 100;
    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    char* format = "Game [ Gen = %lld, Alive = %lld, Died = %lld, Revived = %lld ]";
    
    snprintf(buffer, 
            BUFFER_SIZE, 
            format,
            game->gen.gen,
            game->gen.alive,
            game->gen.died,
            game->gen.revived);

    free(format);

    return buffer;
}

static void copy_cells(const uint8_t* from, uint8_t* to, uint64_t length) {
    for (uint64_t i = 0; i < length; ++i) {
        to[i] = from[i];
    }
}

static game_t* copy_game(const game_t* game) {
    uint64_t length = game->width * game->height;
    uint8_t* cells = calloc(game->width * game->height, sizeof(uint8_t));
    
    copy_cells(game->cells, cells, length);

    game_t* copy = game_create(cells, game->width, game->height);

    copy->gen.died = game->gen.died;
    copy->gen.revived = game->gen.revived;
    copy->gen.gen = game->gen.gen;

    return copy;
}

static void update_game(const game_t* new_game, game_t* game) {
    ensure_games_same_size(new_game, game);

    uint64_t length = new_game->width * new_game->height;
    
    copy_cells(new_game->cells, game->cells, length);
}

static void set_cell(game_t* game, cell_t cell, uint8_t value) {
    ensure_cell_in_bounds(game, cell);

    game->cells[cell.y * game->width + cell.x] = value;
}

static uint8_t get_cell(const game_t* game, cell_t cell) {
    ensure_cell_in_bounds(game, cell);

    return game->cells[cell.y * game->width + cell.x];
}

static uint8_t apply_rules(const game_t* game, game_t* out_game, cell_t cell) {
    uint8_t alive = is_alive(game, cell);
    uint8_t* neighbours = get_neighbours(game, cell);
    uint8_t alive_neighbours = nr_alive_neighbours(neighbours);

    uint8_t state = NOTHING;

    if (alive) {
        /* Any live cell with less than two or more than three live neighbours dies */
        if (alive_neighbours < 2 || alive_neighbours > 3) {
            set_cell(out_game, cell, DEAD);
            state = DIED;
        }
    } else {
        /* Any dead cell with three live neighbours becomes a live cell */
        if (alive_neighbours == 3) {
            set_cell(out_game, cell, ALIVE);
            state = REVIVED;
        }
    }

    free(neighbours);

    return state;
}

static uint8_t* get_neighbours(const game_t* game, cell_t cell) {
    uint8_t* neighbours = calloc(NR_OF_NEIGHBOURS, sizeof(uint8_t));

    uint8_t i = 0;
    int32_t x_max_value = fmin(cell.x + NEIGHBOUR_RADIUS, game->width - 1);
    int32_t y_max_value = fmin(cell.y + NEIGHBOUR_RADIUS, game->height - 1);
    for (uint64_t y = fmax(0, cell.y - NEIGHBOUR_RADIUS); y <= y_max_value; ++y) {
        for (uint64_t x = fmax(0, cell.x - NEIGHBOUR_RADIUS); x <= x_max_value; ++x) {
            if (x != cell.x || y != cell.y) {
                cell_t l_cell = {
                    .x = x,
                    .y = y,
                };
                
                neighbours[i] = get_cell(game, l_cell);
                ++i;
            }
        }
    }

    return neighbours;
}

static uint8_t nr_alive_neighbours(const uint8_t* neighbours) {
    uint8_t count = 0;

    for (uint8_t i = 0; i < NR_OF_NEIGHBOURS; ++i) {
        if (neighbours[i]) {
            ++count;
        }
    }

    return count;
}

static uint64_t nr_alive_cells(const game_t* game) {
    uint64_t count = 0;
    uint64_t length = game->width * game->height;

    for (uint64_t i = 0; i < length; ++i) {
        if (game->cells[i] == ALIVE) {
            ++count;
        }
    }

    return count;
}

static void ensure_cell_in_bounds(const game_t* game, cell_t cell) {    
    if (cell.x >= game->width || cell.y >= game->height) {
        printf_err("Tried to access cell out of bounds. Width=%zu / Height=%zu, X=%llu / Y=%llu.", 
                    game->width, game->height, cell.x, cell.y);
        
        exit(EXIT_FAILURE);
    }
}

static void ensure_games_same_size(const game_t* g1, const game_t* g2) {
    if (g1->width != g2->width || g1->height != g2->height) {
        printf_err("Games not same size. G1: %lldx%lld, G2: %lldx%lld",
                    g1->width, g1->height, g2->width, g2->height);
        
        exit(EXIT_FAILURE);
    }
}
